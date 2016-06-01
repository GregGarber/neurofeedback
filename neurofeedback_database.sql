-- MySQL dump 10.15  Distrib 10.0.24-MariaDB, for debian-linux-gnu (x86_64)
--
-- Host: localhost    Database: neurofeedback
-- ------------------------------------------------------
-- Server version	10.0.24-MariaDB-7

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `alpha_beta`
--

DROP TABLE IF EXISTS `alpha_beta`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `alpha_beta` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `session_id` int(11) DEFAULT NULL,
  `left_alpha` double DEFAULT NULL,
  `right_alpha` double DEFAULT NULL,
  `left_beta` double DEFAULT NULL,
  `right_beta` double DEFAULT NULL,
  `left_power` double DEFAULT NULL,
  `right_power` double DEFAULT NULL,
  `msecs` bigint(20) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=289318 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `channel`
--

DROP TABLE IF EXISTS `channel`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `channel` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `open_vibe_idx` int(11) NOT NULL,
  `ten_twenty` varchar(45) NOT NULL,
  `title` varchar(45) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Temporary table structure for view `corr_stats`
--

DROP TABLE IF EXISTS `corr_stats`;
/*!50001 DROP VIEW IF EXISTS `corr_stats`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE TABLE `corr_stats` (
  `session_id` tinyint NOT NULL,
  `created` tinyint NOT NULL,
  `lalb_corr` tinyint NOT NULL,
  `rarb_corr` tinyint NOT NULL,
  `lara_corr` tinyint NOT NULL,
  `larb_corr` tinyint NOT NULL,
  `lbra_corr` tinyint NOT NULL,
  `lbrb_corr` tinyint NOT NULL
) ENGINE=MyISAM */;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `frequencies`
--

DROP TABLE IF EXISTS `frequencies`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `frequencies` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(45) NOT NULL,
  `low` double NOT NULL,
  `high` double NOT NULL,
  `is_normalized` tinyint(1) NOT NULL DEFAULT '1',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Temporary table structure for view `main_stats`
--

DROP TABLE IF EXISTS `main_stats`;
/*!50001 DROP VIEW IF EXISTS `main_stats`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE TABLE `main_stats` (
  `session_id` tinyint NOT NULL,
  `created` tinyint NOT NULL,
  `raGrb` tinyint NOT NULL,
  `lbGla` tinyint NOT NULL,
  `lbGrb` tinyint NOT NULL,
  `raGla` tinyint NOT NULL,
  `bGa` tinyint NOT NULL,
  `lGr` tinyint NOT NULL,
  `lpGrp` tinyint NOT NULL,
  `lbGra` tinyint NOT NULL,
  `raGlb` tinyint NOT NULL,
  `cnt` tinyint NOT NULL,
  `ms` tinyint NOT NULL,
  `lp_mean` tinyint NOT NULL,
  `rp_mean` tinyint NOT NULL,
  `la_mean` tinyint NOT NULL,
  `lb_mean` tinyint NOT NULL,
  `ra_mean` tinyint NOT NULL,
  `rb_mean` tinyint NOT NULL,
  `la_stdev` tinyint NOT NULL,
  `lb_stdev` tinyint NOT NULL,
  `ra_stdev` tinyint NOT NULL,
  `rb_stdev` tinyint NOT NULL
) ENGINE=MyISAM */;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `nf_session`
--

DROP TABLE IF EXISTS `nf_session`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `nf_session` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `type` varchar(45) NOT NULL,
  `created` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=83 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Temporary table structure for view `percent_stats`
--

DROP TABLE IF EXISTS `percent_stats`;
/*!50001 DROP VIEW IF EXISTS `percent_stats`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE TABLE `percent_stats` (
  `session_id` tinyint NOT NULL,
  `created` tinyint NOT NULL,
  `raGrb` tinyint NOT NULL,
  `lbGla` tinyint NOT NULL,
  `lbGrb` tinyint NOT NULL,
  `raGla` tinyint NOT NULL,
  `bGa` tinyint NOT NULL,
  `lGr` tinyint NOT NULL,
  `lpGrp` tinyint NOT NULL,
  `lbGra` tinyint NOT NULL,
  `raGlb` tinyint NOT NULL
) ENGINE=MyISAM */;
SET character_set_client = @saved_cs_client;

--
-- Final view structure for view `corr_stats`
--

/*!50001 DROP TABLE IF EXISTS `corr_stats`*/;
/*!50001 DROP VIEW IF EXISTS `corr_stats`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `corr_stats` AS select `bs`.`session_id` AS `session_id`,`bs`.`created` AS `created`,(sum(((`alpha_beta`.`left_alpha` - `bs`.`la_mean`) * (`alpha_beta`.`left_beta` - `bs`.`lb_mean`))) / ((`bs`.`cnt` - 1) * (`bs`.`la_stdev` * `bs`.`lb_stdev`))) AS `lalb_corr`,(sum(((`alpha_beta`.`right_alpha` - `bs`.`ra_mean`) * (`alpha_beta`.`right_beta` - `bs`.`rb_mean`))) / ((`bs`.`cnt` - 1) * (`bs`.`ra_stdev` * `bs`.`rb_stdev`))) AS `rarb_corr`,(sum(((`alpha_beta`.`left_alpha` - `bs`.`la_mean`) * (`alpha_beta`.`right_alpha` - `bs`.`ra_mean`))) / ((`bs`.`cnt` - 1) * (`bs`.`la_stdev` * `bs`.`ra_stdev`))) AS `lara_corr`,(sum(((`alpha_beta`.`left_alpha` - `bs`.`la_mean`) * (`alpha_beta`.`right_beta` - `bs`.`rb_mean`))) / ((`bs`.`cnt` - 1) * (`bs`.`la_stdev` * `bs`.`rb_stdev`))) AS `larb_corr`,(sum(((`alpha_beta`.`left_beta` - `bs`.`lb_mean`) * (`alpha_beta`.`right_alpha` - `bs`.`ra_mean`))) / ((`bs`.`cnt` - 1) * (`bs`.`lb_stdev` * `bs`.`ra_stdev`))) AS `lbra_corr`,(sum(((`alpha_beta`.`left_beta` - `bs`.`lb_mean`) * (`alpha_beta`.`right_beta` - `bs`.`rb_mean`))) / ((`bs`.`cnt` - 1) * (`bs`.`lb_stdev` * `bs`.`rb_stdev`))) AS `lbrb_corr` from (`alpha_beta` join `main_stats` `bs` on((`bs`.`session_id` = `alpha_beta`.`session_id`))) group by `alpha_beta`.`session_id` order by `bs`.`created` */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `main_stats`
--

/*!50001 DROP TABLE IF EXISTS `main_stats`*/;
/*!50001 DROP VIEW IF EXISTS `main_stats`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `main_stats` AS select `alpha_beta`.`session_id` AS `session_id`,`s`.`created` AS `created`,sum((case when (`alpha_beta`.`right_alpha` > `alpha_beta`.`right_beta`) then 1 else 0 end)) AS `raGrb`,sum((case when (`alpha_beta`.`left_beta` > `alpha_beta`.`left_alpha`) then 1 else 0 end)) AS `lbGla`,sum((case when (`alpha_beta`.`left_beta` > `alpha_beta`.`right_beta`) then 1 else 0 end)) AS `lbGrb`,sum((case when (`alpha_beta`.`right_alpha` > `alpha_beta`.`left_alpha`) then 1 else 0 end)) AS `raGla`,sum((case when ((`alpha_beta`.`right_beta` + `alpha_beta`.`left_beta`) > (`alpha_beta`.`right_alpha` + `alpha_beta`.`left_alpha`)) then 1 else 0 end)) AS `bGa`,sum((case when ((`alpha_beta`.`left_beta` + `alpha_beta`.`left_alpha`) > (`alpha_beta`.`right_alpha` + `alpha_beta`.`left_alpha`)) then 1 else 0 end)) AS `lGr`,sum((case when (`alpha_beta`.`left_power` > `alpha_beta`.`right_power`) then 1 else 0 end)) AS `lpGrp`,sum((case when (`alpha_beta`.`left_beta` > `alpha_beta`.`right_alpha`) then 1 else 0 end)) AS `lbGra`,sum((case when (`alpha_beta`.`left_power` > `alpha_beta`.`right_power`) then 1 else 0 end)) AS `raGlb`,count(0) AS `cnt`,max(`alpha_beta`.`msecs`) AS `ms`,avg(`alpha_beta`.`left_power`) AS `lp_mean`,avg(`alpha_beta`.`right_power`) AS `rp_mean`,avg(`alpha_beta`.`left_alpha`) AS `la_mean`,avg(`alpha_beta`.`left_beta`) AS `lb_mean`,avg(`alpha_beta`.`right_alpha`) AS `ra_mean`,avg(`alpha_beta`.`right_beta`) AS `rb_mean`,std(`alpha_beta`.`left_alpha`) AS `la_stdev`,std(`alpha_beta`.`left_beta`) AS `lb_stdev`,std(`alpha_beta`.`right_alpha`) AS `ra_stdev`,std(`alpha_beta`.`right_beta`) AS `rb_stdev` from (`alpha_beta` join `nf_session` `s` on((`s`.`id` = `alpha_beta`.`session_id`))) group by `alpha_beta`.`session_id` */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `percent_stats`
--

/*!50001 DROP TABLE IF EXISTS `percent_stats`*/;
/*!50001 DROP VIEW IF EXISTS `percent_stats`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `percent_stats` AS select `t`.`session_id` AS `session_id`,`t`.`created` AS `created`,((`t`.`raGrb` / `t`.`cnt`) * 100) AS `raGrb`,((`t`.`lbGla` / `t`.`cnt`) * 100) AS `lbGla`,((`t`.`lbGrb` / `t`.`cnt`) * 100) AS `lbGrb`,((`t`.`raGla` / `t`.`cnt`) * 100) AS `raGla`,((`t`.`bGa` / `t`.`cnt`) * 100) AS `bGa`,((`t`.`lGr` / `t`.`cnt`) * 100) AS `lGr`,((`t`.`lpGrp` / `t`.`cnt`) * 100) AS `lpGrp`,((`t`.`lbGra` / `t`.`cnt`) * 100) AS `lbGra`,((`t`.`raGlb` / `t`.`cnt`) * 100) AS `raGlb` from `main_stats` `t` */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2016-06-01  8:18:40
