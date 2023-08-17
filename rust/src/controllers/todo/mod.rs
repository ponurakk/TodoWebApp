use actix_web::web::{ServiceConfig, self};
use sea_orm::Order;
use serde::{Deserialize, Serialize};

mod list_todos;
mod get_todo;
mod add_todo;
mod update_todo;
mod remove_todo;

pub const DEFAULT_PAGE_LIMIT: u64 = 3;

pub fn configure(config: &mut ServiceConfig) {
    config
        .route("/todos", web::get().to(list_todos::list_todos))
        .route("/todo", web::post().to(add_todo::add_todo))
        .service(
            web::resource("/todo/{id}")
                .route(web::get().to(get_todo::get_todo))
                .route(web::patch().to(update_todo::update_todo))
                .route(web::delete().to(remove_todo::remove_todo))
        );
}

#[derive(Default, Serialize, Deserialize, Debug, Clone)]
pub enum SortDirection {
    #[default] ASC,
    DESC,
}

#[derive(Serialize, Deserialize, Debug, Clone)]
pub struct TodoQuery {
    pub sort: Option<SortDirection>,
    pub page_limit: Option<u64>,
}

impl SortDirection {
    pub fn to_order(&self) -> Order {
        match *self {
            Self::ASC => Order::Asc,
            Self::DESC => Order::Desc,
        }
    }
}

#[derive(Debug, Deserialize)]
pub struct TodoEntity {
    title: String,
    text: String,
}

#[derive(Deserialize)]
pub struct TodoEntityUpdate {
    title: Option<String>,
    text: Option<String>,
    is_done: Option<bool>,
}
