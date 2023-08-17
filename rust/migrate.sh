DATABASE_URL="sqlite:./sqlite.db?mode=rwc" sea migrate refresh
sea generate entity -u "sqlite:./sqlite.db?mode=rwc" -o src/entities --with-serde both
