import sqlite from 'sqlite';

const db = sqlite.open('./db/database.db').then(db => {
  db.migrate({migrationsPath:'./db/migrate/'})
});

export default db;
