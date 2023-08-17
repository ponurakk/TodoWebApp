use actix_web::http::header::ContentType;
use actix_web::{web, Responder, HttpResponse};
use lib::{AppState, ApiError};
use sea_orm::{EntityTrait, QueryOrder};
use tera::Context;

use crate::controllers::todo::{TodoQuery, DEFAULT_PAGE_LIMIT};
use crate::entities::todo;

pub async fn index(
    query: web::Query<TodoQuery>,
    data: web::Data<AppState>
) -> Result<impl Responder, ApiError> {
    let query = query.into_inner();
    let todos = get_files(query.clone(), &data).await?;

    let mut context = Context::new();
    context.insert("config", &data.config);
    context.insert("todos", &todos);
    context.insert("query", &query);
    let renderer = data.tera.render("index.html", &context)
        .map_err(|e| {
            log::error!("{:#?}", e);
            actix_web::error::ErrorInternalServerError("Template error")
        })?;

    Ok(
        HttpResponse::Ok().content_type(ContentType::html()).body(renderer)
    )
}

async fn get_files(
    query: TodoQuery,
    data: &web::Data<AppState>
) -> Result<Vec<Vec<todo::Model>>, ApiError> {

    let todos = todo::Entity::find()
        .order_by(todo::Column::Id, query.sort.unwrap_or_default().to_order())
        .all(&data.conn)
        .await?;

    Ok(todos
        .chunks(query.page_limit.unwrap_or(DEFAULT_PAGE_LIMIT) as usize)
        .map(|v| v.into())
        .collect::<Vec<Vec<todo::Model>>>())
}
