SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

DROP SCHEMA IF EXISTS `computing` ;
CREATE SCHEMA IF NOT EXISTS `computing` DEFAULT CHARACTER SET latin1 COLLATE latin1_swedish_ci ;
USE `computing` ;

-- -----------------------------------------------------
-- Table `computing`.`prices`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `computing`.`prices` ;

CREATE  TABLE IF NOT EXISTS `computing`.`prices` (
  `Band` INT NOT NULL ,
  `price` DOUBLE NULL ,
  PRIMARY KEY (`Band`) ,
  UNIQUE INDEX `Band_UNIQUE` (`Band` ASC) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `computing`.`Seats`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `computing`.`Seats` ;

CREATE  TABLE IF NOT EXISTS `computing`.`Seats` (
  `seat` VARCHAR(5) NOT NULL ,
  `disabled` INT NULL ,
  `priceBand` INT NOT NULL ,
  PRIMARY KEY (`seat`, `priceBand`) ,
  UNIQUE INDEX `seat_UNIQUE` (`seat` ASC) ,
  INDEX `fk_Seats_prices1_idx` (`priceBand` ASC) ,
  CONSTRAINT `fk_Seats_prices1`
    FOREIGN KEY (`priceBand` )
    REFERENCES `computing`.`prices` (`Band` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `computing`.`Customer`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `computing`.`Customer` ;

CREATE  TABLE IF NOT EXISTS `computing`.`Customer` (
  `REF` VARCHAR(45) NOT NULL ,
  `fName` VARCHAR(20) NOT NULL ,
  `sName` VARCHAR(20) NULL ,
  `telNo` VARCHAR(11) NOT NULL ,
  `eMail` VARCHAR(80) NOT NULL ,
  `passwd` VARCHAR(45) NOT NULL ,
  PRIMARY KEY (`REF`) ,
  UNIQUE INDEX `eMail_UNIQUE` (`eMail` ASC) ,
  UNIQUE INDEX `REF_UNIQUE` (`REF` ASC) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `computing`.`Administrators`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `computing`.`Administrators` ;

CREATE  TABLE IF NOT EXISTS `computing`.`Administrators` (
  `username` VARCHAR(45) NOT NULL ,
  `passwd` VARCHAR(45) NOT NULL ,
  PRIMARY KEY (`username`) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `computing`.`Events`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `computing`.`Events` ;

CREATE  TABLE IF NOT EXISTS `computing`.`Events` (
  `ID` VARCHAR(20) NOT NULL ,
  PRIMARY KEY (`ID`) ,
  UNIQUE INDEX `ID_UNIQUE` (`ID` ASC) )
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `computing`.`Bookings`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `computing`.`Bookings` ;

CREATE  TABLE IF NOT EXISTS `computing`.`Bookings` (
  `Seats_seat` VARCHAR(5) NOT NULL ,
  `Customer_REF` VARCHAR(45) NOT NULL ,
  `Events_ID` VARCHAR(20) NOT NULL ,
  PRIMARY KEY (`Seats_seat`, `Customer_REF`, `Events_ID`) ,
  INDEX `fk_Bookings_Customer1_idx` (`Customer_REF` ASC) ,
  INDEX `fk_Bookings_Events1_idx` (`Events_ID` ASC) ,
  CONSTRAINT `fk_Bookings_Seats`
    FOREIGN KEY (`Seats_seat` )
    REFERENCES `computing`.`Seats` (`seat` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Bookings_Customer1`
    FOREIGN KEY (`Customer_REF` )
    REFERENCES `computing`.`Customer` (`REF` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Bookings_Events1`
    FOREIGN KEY (`Events_ID` )
    REFERENCES `computing`.`Events` (`ID` )
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;



SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;

-- -----------------------------------------------------
-- Data for table `computing`.`prices`
-- -----------------------------------------------------
START TRANSACTION;
USE `computing`;
INSERT INTO `computing`.`prices` (`Band`, `price`) VALUES (1, 10);
INSERT INTO `computing`.`prices` (`Band`, `price`) VALUES (2, 12.5);
INSERT INTO `computing`.`prices` (`Band`, `price`) VALUES (3, 7.25);

COMMIT;

-- -----------------------------------------------------
-- Data for table `computing`.`Seats`
-- -----------------------------------------------------
START TRANSACTION;
USE `computing`;
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('A01', 1, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('A02', NULL, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('A03', NULL, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('A04', NULL, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('A05', NULL, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('A06', NULL, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('A07', NULL, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('A08', NULL, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('A09', NULL, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('A10', NULL, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('A11', 1, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('A12', 1, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('A13', NULL, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('A14', 1, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('B01', 1, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('B02', NULL, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('B03', NULL, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('B04', NULL, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('B05', NULL, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('B06', NULL, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('B07', NULL, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('B08', NULL, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('B09', NULL, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('B10', NULL, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('B11', NULL, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('B12', 1, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('B13', 1, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('B14', NULL, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('B15', NULL, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('B16', 1, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('C01', 1, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('C02', NULL, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('C03', NULL, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('C04', NULL, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('C05', NULL, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('C06', NULL, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('C07', NULL, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('C08', NULL, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('C09', NULL, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('C10', NULL, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('C11', NULL, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('C12', NULL, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('C13', 1, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('C14', 1, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('C15', NULL, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('C16', NULL, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('C17', 1, 1);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('D01', 1, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('D02', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('D03', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('D04', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('D05', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('D06', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('D07', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('D08', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('D09', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('D10', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('D11', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('D12', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('D13', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('D14', 1, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('D15', 1, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('D16', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('D17', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('D18', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('D19', 1, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('E01', 1, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('E02', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('E03', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('E04', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('E05', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('E06', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('E07', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('E08', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('E09', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('E10', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('E11', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('E12', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('E13', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('E14', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('E15', 1, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('E16', 1, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('E17', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('E18', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('E19', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('E20', 1, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('F01', 1, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('F02', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('F03', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('F04', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('F05', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('F06', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('F07', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('F08', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('F09', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('F10', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('F11', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('F12', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('F13', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('F14', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('F15', 1, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('F16', 1, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('F17', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('F18', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('F19', NULL, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('F20', 1, 2);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('G01', 1, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('G02', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('G03', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('G04', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('G05', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('G06', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('G07', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('G08', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('G09', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('G10', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('G11', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('G12', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('G13', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('G14', 1, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('G15', 1, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('G16', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('G17', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('G18', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('G19', 1, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('H01', 1, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('H02', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('H03', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('H04', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('H05', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('H06', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('H07', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('H08', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('H09', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('H10', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('H11', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('H12', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('H13', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('H14', 1, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('H15', 1, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('H16', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('H17', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('H18', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('H19', 1, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('J01', 1, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('J02', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('J03', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('J04', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('J05', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('J06', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('J07', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('J08', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('J09', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('J10', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('J11', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('J12', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('J13', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('J14', 1, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('J15', 1, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('J16', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('J17', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('J18', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('J19', 1, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('K01', 1, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('K02', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('K03', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('K04', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('K05', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('K06', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('K07', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('K08', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('K09', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('K10', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('K11', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('K12', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('K13', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('K14', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('K15', 1, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('K16', 1, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('K17', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('K18', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('K19', 1, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('L01', 1, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('L02', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('L03', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('L04', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('L05', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('L06', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('L07', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('L08', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('L09', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('L10', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('L11', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('L12', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('L13', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('L14', NULL, 3);
INSERT INTO `computing`.`Seats` (`seat`, `disabled`, `priceBand`) VALUES ('L15', 1, 3);

COMMIT;

-- -----------------------------------------------------
-- Data for table `computing`.`Administrators`
-- -----------------------------------------------------
START TRANSACTION;
USE `computing`;
INSERT INTO `computing`.`Administrators` (`username`, `passwd`) VALUES ('admin', 'password');

COMMIT;
