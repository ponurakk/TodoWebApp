use std::{fs::File, io::Read};

use actix_web::{HttpResponse, http::{header::ContentType, StatusCode}, body};
use sea_orm::DatabaseConnection;
use serde::{Deserialize, Serialize};
use serde_json::json;
use tera::Tera;
use thiserror::Error;

#[derive(Error, Debug)]
pub enum ApiError {
    #[error("Database Error: {0}")]
    DbError(#[from] sea_orm::DbErr),
    #[error("Io Error: {0}")]
    IoError(#[from] std::io::Error),
    #[error("{0}")]
    ActixError(#[from] actix_web::Error),
    #[error("Error while serializing or deserializing JSON: {0}")]
    SerdeError(#[from] serde_json::Error),
    #[error("\"{0}\" is Null")]
    NoneValue(&'static str),
    #[error("Todo item with that id doesn't exist")]
    TodoDoesntExist,
}

impl ApiError {
    fn status_code(&self) -> StatusCode {
        match *self {
            Self::DbError(_)
            | Self::IoError(_)
            | Self::ActixError(_)
            | Self::SerdeError(_) => StatusCode::INTERNAL_SERVER_ERROR,
            Self::NoneValue(_)
            | Self::TodoDoesntExist => StatusCode::NOT_FOUND,
        }
    }
}

impl actix_web::error::ResponseError for ApiError {
    fn error_response(&self) -> HttpResponse<body::BoxBody> {
        HttpResponse::build(self.status_code())
            .insert_header(ContentType::json())
            .json(json!({
                "code": self.status_code().as_u16(),
                "message": self.to_string()
            }))
    }

    fn status_code(&self) -> StatusCode {
        match *self {
            Self::DbError(_)
            | Self::IoError(_)
            | Self::ActixError(_)
            | Self::SerdeError(_) => StatusCode::INTERNAL_SERVER_ERROR,
            Self::NoneValue(_)
            | Self::TodoDoesntExist => StatusCode::NOT_FOUND,
        }
    }
}

pub struct AppState {
    pub tera: Tera,
    pub conn: DatabaseConnection,
    pub config: Config,
}

#[derive(Serialize, Deserialize, Clone)]
pub struct Config {
    pub addr: String,
    pub port: u16,
    pub database: String,
}

pub fn get_config() -> Result<Config, std::io::Error> {
    let mut file: File = File::open("config.json")?;
    let mut data: String = String::new();
    file.read_to_string(&mut data)?;
    let json: Config = serde_json::from_str(&data)?;
    Ok(json)
}
