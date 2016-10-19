
-- A database for storing plant soil moisture and ambient temperature

CREATE DATABASE plant_monitoring;
USE plant_monitoring;

-- This table stores information about a plant.
CREATE TABLE plant_monitoring.`plants` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` char(50) DEFAULT NULL,
  `location` char(30) DEFAULT NULL,
  `climate` enum ('inside','outside') DEFAULT 'inside',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- This table stores values read from sensors. The sensors are read
-- periodically by a sensor node and the values stored as a single row
-- with the date and time added by the database server.
CREATE TABLE plant_monitoring.`readings` (
  `id` int(11) NOT NULL,
  `moisture` float DEFAULT NULL,
  `temperature` float DEFAULT NULL,
  `event_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `soil_status` enum ('DRY', 'OK', 'WET') DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

DELIMITER //
CREATE TRIGGER set_status BEFORE INSERT ON readings FOR EACH ROW
BEGIN
  IF NEW.moisture < 250 THEN
    SET NEW.soil_status = 1;
  ELSEIF NEW.moisture < 400 THEN
    SET NEW.soil_status = 2;
  ELSE
    SET NEW.soil_status = 3; 
  END IF;
END //

CREATE FUNCTION plant_monitoring.max_samples_today (in_id int)
RETURNS int DETERMINISTIC READS SQL DATA
BEGIN
  DECLARE num_samples int;
  SELECT COUNT(*) into num_samples FROM plant_monitoring.readings
  WHERE DATE(event_time) = CURRENT_DATE() AND readings.id = in_id;
  RETURN num_samples;
END //
DELIMITER ;

CREATE VIEW plant_monitoring.soil_status_today AS
  SELECT id, soil_status, count(soil_status) as num_events FROM plant_monitoring.readings
  WHERE DATE(event_time) = CURRENT_DATE() GROUP BY id, soil_status;

-- Sample Data --

INSERT INTO plant_monitoring.plants VALUES (NULL, 'Jerusalem Cherry', 'deck', 2);
INSERT INTO plant_monitoring.plants VALUES (NULL, 'Moses in the Cradle', 'patio', 2);
INSERT INTO plant_monitoring.plants VALUES (NULL, 'Peace Lilly', 'porch', 1);
INSERT INTO plant_monitoring.plants VALUES (NULL, 'Thanksgiving Cactus', 'porch', 1);
INSERT INTO plant_monitoring.plants VALUES (NULL, 'African Violet', 'porch', 1);

INSERT INTO plant_monitoring.readings VALUES (1, 235, 39.9, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (1, 235, 38.7, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (1, 230, 38.8, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (1, 230, 39.1, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (1, 215, 39.2, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (1, 215, 39.5, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (1, 225, 39.2, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (1, 220, 38.9, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (1, 222, 38.5, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (1, 218, 37.1, NULL, NULL);

INSERT INTO plant_monitoring.readings VALUES (2, 355, 38.1, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (2, 350, 38.6, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (2, 366, 38.7, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (2, 378, 38.8, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (2, 361, 38.7, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (2, 348, 37.5, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (2, 343, 39.1, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (2, 342, 38.8, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (2, 358, 36.9, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (2, 377, 36.1, NULL, NULL);

INSERT INTO plant_monitoring.readings VALUES (3, 155, 33.6, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (3, 150, 33.7, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (3, 166, 33.6, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (3, 278, 32.3, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (3, 261, 31.2, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (3, 248, 32.5, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (3, 313, 33.6, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (3, 342, 32.8, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (3, 458, 31.9, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (3, 470, 33.4, NULL, NULL);

INSERT INTO plant_monitoring.readings VALUES (4, 333, 33.1, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (4, 345, 33.6, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (4, 360, 34.4, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (4, 380, 34.2, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (4, 395, 33.7, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (4, 385, 33.4, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (4, 425, 32.3, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (4, 420, 31.1, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (4, 422, 33.8, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (4, 418, 32.5, NULL, NULL);

INSERT INTO plant_monitoring.readings VALUES (5, 335, 39.9, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (5, 335, 38.7, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (5, 330, 38.8, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (5, 330, 39.1, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (5, 315, 39.2, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (5, 315, 39.5, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (5, 325, 39.2, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (5, 320, 38.9, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (5, 322, 38.5, NULL, NULL);
INSERT INTO plant_monitoring.readings VALUES (5, 318, 37.1, NULL, NULL);

-- Test queries --

-- Simple Query --

SELECT name, location, moisture
FROM plants
JOIN readings ON plants.id = readings.id
WHERE location LIKE '%deck%';

SELECT * FROM plants
WHERE climate = 2;

SELECT id, AVG(temperature)
FROM readings
WHERE DATE(event_time) = CURRENT_DATE() GROUP BY id;

SELECT name, location, AVG(temperature) as avg_temp
FROM plants
JOIN readings ON plants.id = readings.id
WHERE DATE(event_time) = CURRENT_DATE() AND plants.climate = 2 GROUP BY plants.id;

-- Complex Query --

SELECT *
FROM soil_status_today;

SELECT id, soil_status, num_events, (num_events/max_samples_today(id)) as percent_occurrence
FROM soil_status_today;

SELECT id, soil_status, num_events, (num_events/max_samples_today(id)) as percent_occurrence
FROM soil_status_today
WHERE (num_events/max_samples_today(id)) > 0.50;

SELECT id, soil_status, num_events, (num_events/max_samples_today(id)) as percent_occurrence
FROM soil_status_today
WHERE (num_events/max_samples_today(id)) > 0.50 AND soil_status = 'DRY';

SELECT plants.id, name, location, soil_status, num_events, (num_events/max_samples_today(plants.id)) as percent_occurrence
FROM soil_status_today JOIN plants ON soil_status_today.id = plants.id
WHERE (num_events/max_samples_today(plants.id)) > 0.50 AND soil_status = 1;

SELECT name, location
FROM soil_status_today JOIN plants ON soil_status_today.id = plants.id
WHERE (num_events/max_samples_today(plants.id)) > 0.50 AND soil_status = 1;
