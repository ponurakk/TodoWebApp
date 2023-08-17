use actix_web::web::{ServiceConfig, self};

mod index;

pub fn configure(config: &mut ServiceConfig) {
    config
        .route("/", web::get().to(index::index));
}

