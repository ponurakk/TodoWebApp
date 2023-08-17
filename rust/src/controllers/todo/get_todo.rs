use actix_web::{Responder, web, HttpResponse};
use lib::{ApiError, AppState};
use sea_orm::{EntityTrait, QueryFilter, ColumnTrait};

use crate::entities::todo;

pub async fn get_todo(
    path: web::Path<u64>,
    data: web::Data<AppState>
) -> Result<impl Responder, ApiError> {
    let path = path.into_inner();

    let todo = todo::Entity::find()
        .filter(todo::Column::Id.eq(path))
        .one(&data.conn)
        .await?
        .ok_or(ApiError::TodoDoesntExist)?;

    Ok(HttpResponse::Ok().json(todo))
}
