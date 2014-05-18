<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<head>
	<title>Statistics | The British Museum</title>
	<meta name="viewport" content="width=device-width, initial-scale=1.0"/>
	<meta name="description" content="Home Page"/>
	<meta name="author" content="The British Museum"/>
	<meta name="keywords" content="The British Museum"/>
	<meta name="robots" content="index, follow, archive" />

	<!-- CSS -->
	<?php include $_SERVER['DOCUMENT_ROOT'].'/assets/common/css.php';?>
	
	<!-- Fixes for old browsers -->
	<?php include $_SERVER['DOCUMENT_ROOT'].'/assets/common/fixes.php';?>

</head>

<body>
	<?php include $_SERVER['DOCUMENT_ROOT'].'/assets/common/topnavs/log.php';?>
	
	<?php
	include $_SERVER['DOCUMENT_ROOT'].'/assets/common/database_credentials.php';
	include $_SERVER['DOCUMENT_ROOT'].'/assets/common/database_connect.php';
	?>
	<div class="main-content"><!-- full screen width -->
  <div class="container">
		<h1>Usage Statistics</h1>
		<div class="row">
			<div class="span2">
				<ul class="nav nav-list side-nav">
					<li class="nav-header">Log Manager</li>
					<li class="active"><a href="/log/statistics/">Overview Statistics</a></li>   
					<li class="divider"></li>
					<li><a href="/log/detail/">Detailed Log</a></li>    
					<li class="divider"></li>
					<li><a href="/log/current-location/">Current Customers Location</a></li>    
					<li class="divider"></li>
					<li><a href="/log/average-path/">Average Journey Path</a></li>    
					<li class="divider"></li>    					
				</ul>
            </div> <!-- /span -->
			<div class="span10">
				<p class ="lead">Listed below are statistics regarding the usage of the museum audio guide system.</p>


				<?php
				// Define the most frequent exhibit query .
				$exhibit_query = "SELECT Log.audioCode, Exhibits.name as exhibit FROM Log
				LEFT JOIN Exhibits
				on Log.audioCode=Exhibits.audioCode
				GROUP BY audioCode
				ORDER BY COUNT(*) DESC LIMIT 1";
				
				// Run the query.
				$exhibit_result = mysql_query($exhibit_query);

				// Count the number of sermons returned
				$exhibit_val = mysql_numrows($exhibit_result);

				
				// Check if there were any results
				if ($exhibit_val > 0){
				?>
				
				<p>The most frequently entered Audio Code is <strong>
				<?php
				echo htmlentities(mysql_result($exhibit_result,0,"audioCode"), ENT_QUOTES);
				?>
				</strong> for the exhibit <strong>
				<?php
				echo htmlentities(mysql_result($exhibit_result,0,"exhibit"), ENT_QUOTES);
				?>
				</strong>. </p>
				<br/>

				<?php
				} // there were results
				else {
					// there were no results.
					echo "<p><strong>Sorry, we don&#8217;t appear to have any track usage recorded in the log.</strong></p>";
				}
				?>
				
				<?php
				// Define the most frequent language query .
				$language_query = "SELECT Languages.name as languagename FROM Customers
					LEFT JOIN Languages
					on Customers.language=Languages.code
					GROUP BY language
					ORDER BY COUNT(*) DESC LIMIT 1";
				
				// Run the query.
				$language_result = mysql_query($language_query);

				// Count the number of sermons returned
				$language_val = mysql_numrows($language_result);

				
				// Check if there were any results
				if ($language_val > 0){
				?>

				<p>The most frequently chosen Language is <strong>
				<?php
				echo htmlentities(mysql_result($language_result,0,"languagename"), ENT_QUOTES);
				?>
				</strong>. </p>
				<br/>

				<?php
				} // there were results
				else {
					// there were no results.
					echo "<p><strong>Sorry, we don&#8217;t appear to have any customers using the system.</strong></p>";
				}
				?>

				<?php
				// Define the most frequent expertise query .
				$expertise_query = "SELECT Expertise.name as expertisename FROM Customers
					LEFT JOIN Expertise
					on Customers.expertise=Expertise.levelID
					GROUP BY expertise
					ORDER BY COUNT(*) DESC LIMIT 1";
				
				// Run the query.
				$expertise_result = mysql_query($expertise_query);

				// Count the number of sermons returned
				$expertise_val = mysql_numrows($expertise_result);

				
				// Check if there were any results
				if ($expertise_val > 0){
				?>

				<p>The most frequently chosen Expertise level is <strong>
				<?php
				echo htmlentities(mysql_result($expertise_result,0,"expertisename"), ENT_QUOTES);
				?>
				</strong>. </p>
				<br/>

				<?php
				} // there were results
				else {
					// there were no results.
					echo "<p><strong>Sorry, we don&#8217;t appear to have any customers using the system.</strong></p>";
				}
				?>				

				<?php
				
				/*
				 * Convert seconds to human readable text.
				 *
				 */
				function secs_to_h($secs)
				{
					$units = array(
							"week"   => 7*24*3600,
							"day"    =>   24*3600,
							"hour"   =>      3600,
							"minute" =>        60,
							"second" =>         1,
					);

				// specifically handle zero
					if ( $secs == 0 ) return "0 seconds";

					$s = "";

					foreach ( $units as $name => $divisor ) {
							if ( $quot = intval($secs / $divisor) ) {
									$s .= "$quot $name";
									$s .= (abs($quot) > 1 ? "s" : "") . ", ";
									$secs -= $quot * $divisor;
							}
					}

					return substr($s, 0, -2);
				}

				
				// Define the avergage duration query .
				$duration_query = "SELECT avg(timestampdiff(SECOND, timeIN, timeOUT)) as avgdiff from Bookings";
				
				// Run the query.
				$duration_result = mysql_query($duration_query);

				// Count the number of sermons returned
				$duration_val = mysql_numrows($duration_result);
				
				// Check if there were any results
				if ($duration_val > 0){
				
					$duration = htmlentities(mysql_result($duration_result,0,"avgdiff"), ENT_QUOTES);
					
					?>

					<p>The average time spent in the museum is  <strong>
					<?php
					echo secs_to_h($duration);
					?>
					</strong>. </p>
					<br/>				
					
					<?php
				
				} // there were results

				?>
				
				
			</div>	
		</div>
		<hr/>
	</div><!-- /container -->
</div><!-- /main-content -->
	
	<!-- Footer -->
	<?php include $_SERVER['DOCUMENT_ROOT'].'/assets/common/footer.php';?>
	<!-- /footer -->

	<!-- Scripts, placed at the end of the document so the pages load faster -->
	<?php include $_SERVER['DOCUMENT_ROOT'].'/assets/common/scripts.php';?>

</body>
</html>