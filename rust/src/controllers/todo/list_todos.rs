use actix_web::{Responder, web, HttpResponse};
use lib::{ApiError, AppState};
use sea_orm::{EntityTrait, QueryOrder};

use crate::entities::todo;

use super::TodoQuery;

pub async fn list_todos(
    query: web::Query<TodoQuery>,
    data: web::Data<AppState>
) -> Result<impl Responder, ApiError> {
    let query = query.into_inner();

    let todos = todo::Entity::find()
        .order_by(todo::Column::Id, query.sort.unwrap_or_default().to_order())
        .all(&data.conn)
        .await?;

    let todos: Vec<Vec<todo::Model>> = todos
        .chunks(query.page_limit.unwrap_or_default() as usize)
        .map(|v| v.into())
        .collect();

    Ok(HttpResponse::Ok().json(todos))
}
