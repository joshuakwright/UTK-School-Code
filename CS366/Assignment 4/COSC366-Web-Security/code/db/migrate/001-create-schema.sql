--------------------------------------------------------------------------------
-- Up
--------------------------------------------------------------------------------

CREATE TABLE Users (
  username        TEXT PRIMARY KEY,
  password        TEXT NOT NULL,
  profile         TEXT NOT NULL,
  volCoins         INTEGER NOT NULL
);


--------------------------------------------------------------------------------
-- Down
--------------------------------------------------------------------------------

DROP TABLE Users;
