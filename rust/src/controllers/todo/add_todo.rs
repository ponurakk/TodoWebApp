use actix_web::http::header;
use actix_web::{Responder, web, HttpResponse};
use lib::{ApiError, AppState};
use sea_orm::{EntityTrait, Set};
use serde_json::json;

use crate::entities::todo;

use super::TodoEntity;

pub async fn add_todo(
    body: web::Form<TodoEntity>,
    data: web::Data<AppState>
) -> Result<impl Responder, ApiError> {
    let body = body.into_inner();
    println!("{:#?}", body);

    let todo_model = todo::ActiveModel {
        title: Set(body.title),
        text: Set(body.text),
        is_done: Set(false),
        created_at: Set(chrono::Local::now().timestamp().to_string()),
        updated_at: Set(chrono::Local::now().timestamp().to_string()),
        ..Default::default()
    };

    let todo_inserted = todo::Entity::insert(todo_model).exec(&data.conn).await?;
    

    // Return json in case of disabling follow redirects
    Ok(
        HttpResponse::Found()
            .insert_header((header::LOCATION, "/"))
            .json(json!({
                "last_inserted_id": todo_inserted.last_insert_id
            }))
    )
}
