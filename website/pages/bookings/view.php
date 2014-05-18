<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<head>
	<title>Bookings | The British Museum</title>
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
	<?php include $_SERVER['DOCUMENT_ROOT'].'/assets/common/topnavs/bookings.php';?>
	
	<?php
	include $_SERVER['DOCUMENT_ROOT'].'/assets/common/database_credentials.php';
	include $_SERVER['DOCUMENT_ROOT'].'/assets/common/database_connect.php';
	?>
	<div class="main-content"><!-- full screen width -->
  <div class="container">
		<h1>Bookings Manager</h1>
		<div class="row">
			<div class="span2">
				<ul class="nav nav-list side-nav">
					<li class="nav-header">Bookings Manager</li>
					<li class="active"><a href="/bookings/view-bookings/">View All Bookings</a></li>   
					<li class="divider"></li>
					<li><a href="/bookings/add-booking/">Add Booking</a></li>    
					<li class="divider"></li>
					<li><a href="/bookings/return-unit/">Mark as Returned</a></li>    
					<li class="divider"></li>							
				</ul>
            </div> <!-- /span -->
			<div class="span10">
				<p class ="lead">Listed below are all bookings stored in the database.</p>

				<?php
				// Define the query.
				$query = "SELECT Bookings.bookingID, Bookings.mac, Bookings.timeIN, Bookings.timeOUT, Customers.name as name
				FROM Bookings
				LEFT JOIN Customers
				ON Bookings.customerID=Customers.customerID
				ORDER BY Bookings.timeIN DESC";
				
				// Run the query.
				$result = mysql_query($query);

				// Count the number of sermons returned
				$num = mysql_numrows($result);

				// Close database connection
				mysql_close();
				
				// Check if there were any results
				if ($num > 0){
				?>
				
				<table class='table table-bordered'>
					<thead>
						<tr>
							<th>Booking ID</th>
							<th>Customer Name</th>
							<th>Handheld MAC Address</th>
							<th>Time In</th>
							<th>Time Out</th>
						</tr>
					</thead>
					<tbody>

					<!-- for each row -->
					<?php
					// Loop through each result in turn.  Currently does all results at once.  Perhaps change to limit to n results and then load second set of results with Ajax.
					$i=0;
					while ($i < $num) {
					
					echo"<tr>";
						echo"<td>";
							echo htmlentities(mysql_result($result,$i,"bookingID"), ENT_QUOTES);
						echo"</td>";
						
						echo"<td>";
							echo htmlentities(mysql_result($result,$i,"name"), ENT_QUOTES);
						echo"</td>";
						
						echo"<td>";
							echo htmlentities(mysql_result($result,$i,"mac"), ENT_QUOTES);
						echo"</td>";
						
						echo"<td>";
							echo htmlentities(mysql_result($result,$i,"timeIN"), ENT_QUOTES);
						echo"</td>";
						
						echo"<td>";
							echo htmlentities(mysql_result($result,$i,"timeOUT"), ENT_QUOTES);
						echo"</td>";
					
					echo"</tr>";
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
					echo "<p><strong>Sorry, we don&#8217;t appear to have any bookings.</strong></p>";
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