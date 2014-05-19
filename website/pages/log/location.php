<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<head>
	<title>Current Customers Location | The British Museum</title>
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
		<h1>Current Customers Location</h1>
		<div class="row">
			<div class="span2">
				<ul class="nav nav-list side-nav">
					<li class="nav-header">Log Manager</li>
					<li><a href="/log/statistics/">Overview Statistics</a></li>   
					<li class="divider"></li>
					<li><a href="/log/detail/">Detailed Log</a></li>    
					<li class="divider"></li>
					<li class="active"><a href="/log/current-location/">Current Customers Location</a></li>    
					<li class="divider"></li>
					<li><a href="/log/average-path/">Average Journey Path</a></li>    
					<li class="divider"></li>    					
				</ul>
            </div> <!-- /span -->
			<div class="span10">
				<p class ="lead">Listed below are the locations of all customers currently in the museum.</p>


				<?php
				// Define the query.
				$customer_query = "SELECT Customers.customerID, Customers.name as name
				FROM Bookings
				LEFT JOIN Customers
				ON Bookings.customerID=Customers.customerID
				WHERE Bookings.timeOUT IS NULL
				ORDER BY Customers.name ASC";
				
				// Run the query.
				$customer_result = mysql_query($customer_query);

				// Count the number of sermons returned
				$customer_num = mysql_numrows($customer_result);


				
				// Check if there were any results
				if ($customer_num > 0){


					// Loop through each result in turn.  Currently does all results at once.  Perhaps change to limit to n results and then load second set of results with Ajax.
					$i=0;
					while ($i < $customer_num) {

						$customerID = htmlentities(mysql_result($customer_result,$i,"customerID"), ENT_QUOTES);

							// Define the query.
							$location_query = "SELECT Log.entryID, Log.customerID, Log.dateTime, Log.audioCode, Exhibits.name as exhibit, Exhibits.location as location
								FROM Log
								LEFT JOIN Exhibits
								on Log.audioCode=Exhibits.audioCode
								WHERE Log.customerID=$customerID
								ORDER BY dateTime DESC";
							
								// Run the query.
								$location_result = mysql_query($location_query);

								// Count the number returned
								$location_num = mysql_numrows($location_result);

								if ($location_num > 0){

								echo "<p>Customer ".htmlentities(mysql_result($customer_result,$i,"name"), ENT_QUOTES)." is currently in ".htmlentities(mysql_result($location_result,0,"location"), ENT_QUOTES)." listening to ".htmlentities(mysql_result($location_result,0,"exhibit"), ENT_QUOTES)." .</p>";

							}
							else {
										echo "<p>Customer ".htmlentities(mysql_result($customer_result,$i,"name"), ENT_QUOTES)." is currently in the museum but has not yet listened to any exhibit.  We assume that they are still near to the entrance.</p>";

							}
					


						$i++; // increment the count
				} // while loop going through each file returned.
					?>
					<!-- /for each row -->
					</tbody>
				</table>
				<?php
				} // there were results
				else {
					// there were no results.
					echo "<p><strong>There are currently no customers in the musuem.  The fire alarm can be disabled.</strong></p>";
				}
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