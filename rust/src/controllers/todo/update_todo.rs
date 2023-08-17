use actix_web::{Responder, web, HttpResponse};
use lib::{ApiError, AppState};
use sea_orm::{EntityTrait, Set, QueryFilter, ColumnTrait, ActiveModelTrait};

use crate::entities::todo;

use super::TodoEntityUpdate;

pub async fn update_todo(
    path: web::Path<u64>,
    body: web::Json<TodoEntityUpdate>,
    data: web::Data<AppState>
) -> Result<impl Responder, ApiError> {
    let path = path.into_inner();
    let body = body.into_inner();

    let todo_db = todo::Entity::find()
        .filter(todo::Column::Id.eq(path))
        .one(&data.conn)
        .await?
        .ok_or(ApiError::TodoDoesntExist)?;
    let mut todo_db: todo::ActiveModel = todo_db.into();

    // We can unwrap here safely
    if body.title.is_some() {
        todo_db.title = Set(body.title.unwrap());
    }
    if body.text.is_some() {
        todo_db.text = Set(body.text.unwrap());
    }
    if body.is_done.is_some() {
        todo_db.is_done = Set(body.is_done.unwrap());
    }
    todo_db.updated_at = Set(chrono::Local::now().timestamp().to_string());
    
    let update_res = todo_db.update(&data.conn).await?;

    Ok(HttpResponse::Ok().json(update_res))
}
