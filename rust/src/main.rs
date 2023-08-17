use actix_cors::Cors;
use actix_files::Files;
use actix_web::{HttpServer, App, web, middleware};
use lib::{ApiError, get_config, AppState};
use migration::{Migrator, MigratorTrait};
use sea_orm::{Database, DatabaseConnection};
use tera::Tera;

pub mod entities;
mod controllers;

#[actix_web::main]
async fn main() -> Result<(), ApiError> {
    env_logger::init_from_env(env_logger::Env::new().default_filter_or("info"));

    let config: lib::Config = get_config()?;
    let bind = (config.addr.clone(), config.port.clone());
    let conn: DatabaseConnection = Database::connect(&config.database).await?;

    Migrator::up(&conn, None).await?;
    Migrator::status(&conn).await?;

    HttpServer::new(move || {
        let tera: Tera = Tera::new("templates/**/*").expect("No templates found");

        let state = AppState {
            tera,
            conn: conn.clone(),
            config: config.clone(),
        };

        let cors = Cors::default()
            .allow_any_origin()
            .allow_any_method()
            .allow_any_header();

        App::new()
            .app_data(web::Data::new(state))
            .wrap(middleware::Logger::default().log_target("TodoWebApp"))
            .wrap(cors)
            .service(
                web::scope("/api")
                    .configure(controllers::todo::configure)
            )
            .configure(controllers::website::configure)
            .service(Files::new("/assets", "./assets"))
    })
    .bind(bind)?
    .run()
    .await?;

    Ok(())
}


