CREATE DATABASE IF NOT EXISTS GA_sessions;
CREATE TABLE IF NOT EXISTS GA_sessions.batch (id INT(10) PRIMARY KEY auto_increment, batchId varchar(36), inputFile varchar(128), totalTasks INT(10), started timestamp);
CREATE TABLE IF NOT EXISTS GA_sessions.sessions (id INT(10) PRIMARY KEY auto_increment, server varchar(256), batch int(10), uuid varchar(36), started timestamp default 0, complete double , completed timestamp default 0);
CREATE TABLE IF NOT EXISTS GA_sessions.stats (uuid varchar(36) PRIMARY KEY, mean_score float, mean_tscore float, median_score float, median_tscore float, permutation bool, labels longtext);
