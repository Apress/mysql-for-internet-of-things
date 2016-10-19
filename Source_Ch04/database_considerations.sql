CREATE TABLE `simple_annotation_test` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `temperature` float NOT NULL,
  `barometric` float NOT NULL,
  `notes` char(128) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;

CREATE TABLE `date_annotation_test` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `temperature` float NOT NULL,
  `barometric` float DEFAULT NULL,
  `date_saved` datetime DEFAULT NULL,
  `notes` char(128) DEFAULT NULL,
  `data_changed` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;

ALTER TABLE date_annotation_test ADD COLUMN data_changed TIMESTAMP AFTER notes;

CREATE TABLE `derived_annotation_test` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `blood_oxygen` float NOT NULL,
  `blood_oxygen_corrected` float DEFAULT NULL,
  `weight` float DEFAULT NULL,
  `volts` float DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;

CREATE TRIGGER calibrate_blood_oyxgen BEFORE INSERT ON derived_annotation_test
FOR EACH ROW
SET NEW.blood_oxygen_corrected = NEW.blood_oxygen + 0.785;

INSERT INTO derived_annotation_test (blood_oxygen, weight, volts) VALUES (94.88, 46.07, 68.42);

INSERT INTO derived_annotation_test (blood_oxygen, weight, volts)
VALUES (94.88, 46.07, 68.42), (95.23, 46.56, 68.52), (94.97, 46.42, 68.62);


INSERT INTO derived_annotation_test (blood_oxygen, weight, avg_weight, volts)
VALUES (95.23, 46.56, (46.56/4.00), 68.52);

ALTER TABLE derived_annotation_test ADD COLUMN avg_weight float AFTER weight;

INSERT INTO derived_annotation_test (blood_oxygen, weight, avg_weight, volts)
VALUES (95.46, 46.49, (46.49/4.00), 69.62), (94.96, 46.18, (46.18/4.00), 70.12), (94.62, 46.11, (46.11/4.00), 70.52);

CREATE TABLE `pond` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `water_level` int NOT NULL,
  `state` char(12) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=latin1;

CREATE TRIGGER set_pond_state BEFORE INSERT ON pond FOR EACH ROW
BEGIN
  IF NEW.water_level > 1500 THEN
    SET NEW.state = 'CLEAN';
  ELSEIF NEW.water_level > 1150 THEN
    SET NEW.state = 'LOW';
  ELSEIF NEW.water_level > 700 THEN
    SET NEW.state = 'NORMAL';
  ELSE
    SET NEW.state = 'HIGH'; 
  END IF;
END //

INSERT INTO pond VALUES (null, 1501, null), (null, 1151, null), (null, 701, null), (null, 600, null);

CREATE TABLE `aggregation_test` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `temperature` float NOT NULL,
  `barometric` float NOT NULL,
  `notes` char(128) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;

INSERT INTO aggregation_test (temperature, barometric) VALUES 
(98.6546070643399, 15.122450599843349), (91.29552309974738, 14.972940349831173), (92.40935104377152, 15.145606149820694), (92.87910512847053, 14.948084228430185), (91.65019875973566, 14.791053443295606), (90.39148761246808, 15.229438903966315), (92.08619488530134, 15.167059758781788), (97.0787463589415, 14.68713521117044), (91.2966169699032, 15.425027499725703), (93.90785124530322, 14.892194221850174), (92.11182609830854, 15.117576082504092),(90.30730523055993, 15.119159408093276), (99.18853417359651, 14.724791510827576),(94.58637327109213, 15.228247249252066), (96.45129913507253, 15.435445357179253),(92.98671911139155, 15.151292947338694), (95.81732326868385, 14.718610244009914),(90.27742511006188, 14.812070888231549), (98.1720416235962, 14.776562022843033), (99.8600782018115, 15.196747869978257);

