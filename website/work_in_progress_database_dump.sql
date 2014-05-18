-- phpMyAdmin SQL Dump
-- version 4.1.8
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: May 18, 2014 at 05:21 PM
-- Server version: 5.5.36-cll
-- PHP Version: 5.4.23

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `edwardbc_esd_db`
--

-- --------------------------------------------------------

--
-- Table structure for table `Audio`
--

CREATE TABLE IF NOT EXISTS `Audio` (
  `trackID` int(11) NOT NULL AUTO_INCREMENT,
  `audioCode` int(2) NOT NULL,
  `expertise` int(11) NOT NULL,
  `language` char(3) NOT NULL,
  `filePath` varchar(535) NOT NULL,
  PRIMARY KEY (`trackID`),
  UNIQUE KEY `trackfind` (`audioCode`,`expertise`,`language`),
  KEY `language` (`language`),
  KEY `expertise` (`expertise`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=106 ;

--
-- Dumping data for table `Audio`
--

INSERT INTO `Audio` (`trackID`, `audioCode`, `expertise`, `language`, `filePath`) VALUES
(34, 12, 1, 'eng', 'http://esd.edwardb.co.uk/audio/rosetta/english/novice.mp3'),
(35, 12, 2, 'eng', 'http://esd.edwardb.co.uk/audio/rosetta/english/medium.mp3'),
(36, 12, 3, 'eng', 'http://esd.edwardb.co.uk/audio/rosetta/english/expert.mp3'),
(37, 12, 1, 'fra', 'http://esd.edwardb.co.uk/audio/rosetta/french/novice.mp3'),
(38, 12, 2, 'fra', 'http://esd.edwardb.co.uk/audio/rosetta/french/medium.mp3'),
(39, 12, 3, 'fra', 'http://esd.edwardb.co.uk/audio/rosetta/french/expert.mp3'),
(40, 12, 1, 'deu', 'http://esd.edwardb.co.uk/audio/rosetta/german/novice.mp3'),
(41, 12, 2, 'deu', 'http://esd.edwardb.co.uk/audio/rosetta/german/medium.mp3'),
(42, 12, 3, 'deu', 'http://esd.edwardb.co.uk/audio/rosetta/german/expert.mp3'),
(43, 15, 1, 'eng', 'http://esd.edwardb.co.uk/audio/amenhotep/english/novice.mp3'),
(44, 15, 2, 'eng', 'http://esd.edwardb.co.uk/audio/amenhotep/english/medium.mp3'),
(45, 15, 3, 'eng', 'http://esd.edwardb.co.uk/audio/amenhotep/english/expert.mp3'),
(46, 15, 1, 'fra', 'http://esd.edwardb.co.uk/audio/amenhotep/french/novice.mp3'),
(47, 15, 2, 'fra', 'http://esd.edwardb.co.uk/audio/amenhotep/french/medium.mp3'),
(48, 15, 3, 'fra', 'http://esd.edwardb.co.uk/audio/amenhotep/french/expert.mp3'),
(49, 15, 1, 'deu', 'http://esd.edwardb.co.uk/audio/amenhotep/german/novice.mp3'),
(50, 15, 2, 'deu', 'http://esd.edwardb.co.uk/audio/amenhotep/german/medium.mp3'),
(51, 15, 3, 'deu', 'http://esd.edwardb.co.uk/audio/amenhotep/german/expert.mp3'),
(52, 20, 1, 'eng', 'http://esd.edwardb.co.uk/audio/chessmen/english/novice.mp3'),
(53, 20, 2, 'eng', 'http://esd.edwardb.co.uk/audio/chessmen/english/medium.mp3'),
(54, 20, 3, 'eng', 'http://esd.edwardb.co.uk/audio/chessmen/english/expert.mp3'),
(55, 20, 1, 'fra', 'http://esd.edwardb.co.uk/audio/chessmen/french/novice.mp3'),
(56, 20, 2, 'fra', 'http://esd.edwardb.co.uk/audio/chessmen/french/medium.mp3'),
(57, 20, 3, 'fra', 'http://esd.edwardb.co.uk/audio/chessmen/french/expert.mp3'),
(58, 20, 1, 'deu', 'http://esd.edwardb.co.uk/audio/chessmen/german/novice.mp3'),
(59, 20, 2, 'deu', 'http://esd.edwardb.co.uk/audio/chessmen/german/medium.mp3'),
(60, 20, 3, 'deu', 'http://esd.edwardb.co.uk/audio/chessmen/german/expert.mp3'),
(61, 8, 1, 'eng', 'http://esd.edwardb.co.uk/audio/elgin/english/novice.mp3'),
(62, 8, 2, 'eng', 'http://esd.edwardb.co.uk/audio/elgin/english/medium.mp3'),
(63, 8, 3, 'eng', 'http://esd.edwardb.co.uk/audio/elgin/english/expert.mp3'),
(64, 8, 1, 'fra', 'http://esd.edwardb.co.uk/audio/elgin/french/novice.mp3'),
(65, 8, 2, 'fra', 'http://esd.edwardb.co.uk/audio/elgin/french/medium.mp3'),
(66, 8, 3, 'fra', 'http://esd.edwardb.co.uk/audio/elgin/french/expert.mp3'),
(67, 8, 1, 'deu', 'http://esd.edwardb.co.uk/audio/elgin/german/novice.mp3'),
(68, 8, 2, 'deu', 'http://esd.edwardb.co.uk/audio/elgin/german/medium.mp3'),
(69, 8, 3, 'deu', 'http://esd.edwardb.co.uk/audio/elgin/german/expert.mp3'),
(70, 22, 1, 'eng', 'http://esd.edwardb.co.uk/audio/hakananaia/english/novice.mp3'),
(71, 22, 2, 'eng', 'http://esd.edwardb.co.uk/audio/hakananaia/english/medium.mp3'),
(72, 22, 3, 'eng', 'http://esd.edwardb.co.uk/audio/hakananaia/english/expert.mp3'),
(73, 22, 1, 'fra', 'http://esd.edwardb.co.uk/audio/hakananaia/french/novice.mp3'),
(74, 22, 2, 'fra', 'http://esd.edwardb.co.uk/audio/hakananaia/french/medium.mp3'),
(75, 22, 3, 'fra', 'http://esd.edwardb.co.uk/audio/hakananaia/french/expert.mp3'),
(76, 22, 1, 'deu', 'http://esd.edwardb.co.uk/audio/hakananaia/german/novice.mp3'),
(77, 22, 2, 'deu', 'http://esd.edwardb.co.uk/audio/hakananaia/german/medium.mp3'),
(78, 22, 3, 'deu', 'http://esd.edwardb.co.uk/audio/hakananaia/german/expert.mp3'),
(79, 33, 1, 'eng', 'http://esd.edwardb.co.uk/audio/portland/english/novice.mp3'),
(80, 33, 2, 'eng', 'http://esd.edwardb.co.uk/audio/portland/english/medium.mp3'),
(81, 33, 3, 'eng', 'http://esd.edwardb.co.uk/audio/portland/english/expert.mp3'),
(82, 33, 1, 'fra', 'http://esd.edwardb.co.uk/audio/portland/french/novice.mp3'),
(83, 33, 2, 'fra', 'http://esd.edwardb.co.uk/audio/portland/french/medium.mp3'),
(84, 33, 3, 'fra', 'http://esd.edwardb.co.uk/audio/portland/french/expert.mp3'),
(85, 33, 1, 'deu', 'http://esd.edwardb.co.uk/audio/portland/german/novice.mp3'),
(86, 33, 2, 'deu', 'http://esd.edwardb.co.uk/audio/portland/german/medium.mp3'),
(87, 33, 3, 'deu', 'http://esd.edwardb.co.uk/audio/portland/german/expert.mp3'),
(88, 39, 1, 'eng', 'http://esd.edwardb.co.uk/audio/serpent/english/novice.mp3'),
(89, 39, 2, 'eng', 'http://esd.edwardb.co.uk/audio/serpent/english/medium.mp3'),
(90, 39, 3, 'eng', 'http://esd.edwardb.co.uk/audio/serpent/english/expert.mp3'),
(91, 39, 1, 'fra', 'http://esd.edwardb.co.uk/audio/serpent/french/novice.mp3'),
(92, 39, 2, 'fra', 'http://esd.edwardb.co.uk/audio/serpent/french/medium.mp3'),
(93, 39, 3, 'fra', 'http://esd.edwardb.co.uk/audio/serpent/french/expert.mp3'),
(94, 39, 1, 'deu', 'http://esd.edwardb.co.uk/audio/serpent/german/novice.mp3'),
(95, 39, 2, 'deu', 'http://esd.edwardb.co.uk/audio/serpent/german/medium.mp3'),
(96, 39, 3, 'deu', 'http://esd.edwardb.co.uk/audio/serpent/german/expert.mp3'),
(97, 41, 1, 'eng', 'http://esd.edwardb.co.uk/audio/vindolanda/english/novice.mp3'),
(98, 41, 2, 'eng', 'http://esd.edwardb.co.uk/audio/vindolanda/english/medium.mp3'),
(99, 41, 3, 'eng', 'http://esd.edwardb.co.uk/audio/vindolanda/english/expert.mp3'),
(100, 41, 1, 'fra', 'http://esd.edwardb.co.uk/audio/vindolanda/french/novice.mp3'),
(101, 41, 2, 'fra', 'http://esd.edwardb.co.uk/audio/vindolanda/french/medium.mp3'),
(102, 41, 3, 'fra', 'http://esd.edwardb.co.uk/audio/vindolanda/french/expert.mp3'),
(103, 41, 1, 'deu', 'http://esd.edwardb.co.uk/audio/vindolanda/german/novice.mp3'),
(104, 41, 2, 'deu', 'http://esd.edwardb.co.uk/audio/vindolanda/german/medium.mp3'),
(105, 41, 3, 'deu', 'http://esd.edwardb.co.uk/audio/vindolanda/german/expert.mp3');

-- --------------------------------------------------------

--
-- Table structure for table `Customers`
--

CREATE TABLE IF NOT EXISTS `Customers` (
  `customerID` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(535) NOT NULL,
  `address` varchar(535) NOT NULL,
  `language` char(3) NOT NULL,
  `expertise` int(1) NOT NULL,
  PRIMARY KEY (`customerID`),
  KEY `language` (`language`),
  KEY `expertise` (`expertise`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=14 ;

--
-- Dumping data for table `Customers`
--

INSERT INTO `Customers` (`customerID`, `name`, `address`, `language`, `expertise`) VALUES
(7, 'Edward Bradley', '1 The Road, The Street, Bath', 'eng', 3),
(8, 'Bob Smith', 'Germany', 'deu', 2),
(9, 'Tom Jones', 'England', 'eng', 3),
(10, 'Frank', 'Somerwhere', 'deu', 2),
(11, 'Martin', 'Overthere', 'deu', 1),
(12, 'Prof', 'Here there everywhere', 'eng', 3),
(13, 'Someone', 'Nowhere', 'eng', 3);

-- --------------------------------------------------------

--
-- Table structure for table `Exhibits`
--

CREATE TABLE IF NOT EXISTS `Exhibits` (
  `exhibitID` int(11) NOT NULL AUTO_INCREMENT,
  `audioCode` int(2) NOT NULL,
  `location` varchar(255) NOT NULL,
  `name` varchar(255) NOT NULL,
  PRIMARY KEY (`exhibitID`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=19 ;

--
-- Dumping data for table `Exhibits`
--

INSERT INTO `Exhibits` (`exhibitID`, `audioCode`, `location`, `name`) VALUES
(11, 12, '1R01', 'The Rosetta Stone'),
(12, 15, '2P47', 'Colossal red granite statue of Amenhotep III'),
(13, 20, '3Q21', 'The Lewis Chessmen'),
(14, 8, '2Q21', 'The Elgin Marbles'),
(15, 22, '1L14', 'Hoa Hakananaia'),
(16, 33, '4B28', 'The Portland Vase'),
(17, 39, '2K41', 'The Double-headed serpent'),
(18, 41, '1Q57', 'The Vindolanda tablets');

-- --------------------------------------------------------

--
-- Table structure for table `Expertise`
--

CREATE TABLE IF NOT EXISTS `Expertise` (
  `levelID` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(255) NOT NULL,
  PRIMARY KEY (`levelID`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=6 ;

--
-- Dumping data for table `Expertise`
--

INSERT INTO `Expertise` (`levelID`, `name`) VALUES
(1, 'Novice'),
(2, 'Medium'),
(3, 'Expert');

-- --------------------------------------------------------

--
-- Table structure for table `Languages`
--

CREATE TABLE IF NOT EXISTS `Languages` (
  `code` char(3) NOT NULL,
  `name` varchar(255) NOT NULL,
  PRIMARY KEY (`code`),
  UNIQUE KEY `code` (`code`),
  UNIQUE KEY `name` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `Languages`
--

INSERT INTO `Languages` (`code`, `name`) VALUES
('eng', 'English'),
('fra', 'French'),
('deu', 'German');

-- --------------------------------------------------------

--
-- Table structure for table `Log`
--

CREATE TABLE IF NOT EXISTS `Log` (
  `entryID` int(11) NOT NULL AUTO_INCREMENT,
  `customerID` int(11) NOT NULL,
  `audioCode` int(2) NOT NULL,
  `dateTime` datetime NOT NULL,
  PRIMARY KEY (`entryID`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=4 ;

--
-- Dumping data for table `Log`
--

INSERT INTO `Log` (`entryID`, `customerID`, `audioCode`, `dateTime`) VALUES
(1, 7, 12, '2014-05-18 15:15:43'),
(2, 7, 15, '2014-05-18 15:29:28'),
(3, 4, 12, '2014-05-18 15:58:34');

--
-- Constraints for dumped tables
--

--
-- Constraints for table `Audio`
--
ALTER TABLE `Audio`
  ADD CONSTRAINT `Audio_ibfk_2` FOREIGN KEY (`expertise`) REFERENCES `Expertise` (`levelID`),
  ADD CONSTRAINT `Audio_ibfk_1` FOREIGN KEY (`language`) REFERENCES `Languages` (`code`);

--
-- Constraints for table `Customers`
--
ALTER TABLE `Customers`
  ADD CONSTRAINT `Customers_ibfk_2` FOREIGN KEY (`expertise`) REFERENCES `Expertise` (`levelID`),
  ADD CONSTRAINT `Customers_ibfk_1` FOREIGN KEY (`language`) REFERENCES `Languages` (`code`);

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
