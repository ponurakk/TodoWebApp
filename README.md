# TodoWebApp
Simple todo webapp in most languages i know

### Languages
- [x] 1. Rust (actix, tera, sea-orm)
- [ ] 2. C++ (crow, tinyorm)
- [ ] 3. Rust (axum, tera, sea-orm)
- [ ] 4. Typescript (express, ejs, typeorm)
- [ ] 5. Python (flask, ponyorm)
- [ ] 6. Kotlin (ktor, ktorm)

### Other tools
- TailwindCSS
- SQLite

### Endpoints
#### FrontEnd
- `GET` `/`  -> Main page

#### BackEnd
- `GET` `/api/todos?sort&page-limit` -> Get todo items
- `GET` `/api/todo/:id` -> Get one todo item
- `POST` `/api/todo` -> Add new todo item
- `DELETE` `/api/todo`  -> Remove todo item
- `PUT` `/api/todo` -> Update todo item

