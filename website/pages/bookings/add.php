<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<head>
	<title>Add Booking | The British Museum</title>
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
		<h1>Add Booking</h1>
		<div class="row">
			<div class="span2">
				<ul class="nav nav-list side-nav">
					<li class="nav-header">Bookings Manager</li>
					<li><a href="/bookings/view-bookings/">View All Bookings</a></li>   
					<li class="divider"></li>
					<li class="active"><a href="/bookings/add-booking/">Add Booking</a></li>    
					<li class="divider"></li>
					<li><a href="/bookings/return-unit/">Mark as Returned</a></li>    
					<li class="divider"></li>							
				</ul>
            </div> <!-- /span -->
			<div class="span10">
				<?php
				if (!empty($_REQUEST['name'])) {
					if (!empty($_REQUEST['handheld'])) {
						if (!empty($_REQUEST['authcode'])) {
							$name=$_POST['name'];
							$handheld=$_POST['handheld'];
							$authcode=$_POST['authcode'];
						
						
							$insertbooking = "INSERT INTO Bookings ".
									  "(customerID, mac, timeIN) ".
									  "VALUES ( '$name', '$handheld', (select now()) )";
							
							$flag_booking_inserted = mysql_query($insertbooking);
							
							// Only add to igep table if booking insert above succeeded
							if ( $flag_booking_inserted ) {
							
								$updateauthcode = "UPDATE IGEP SET authCode='$authcode' WHERE mac='$handheld'";
								mysql_query($updateauthcode);
							

							
				?>

				
				<div class="alert alert-success">
					<button type="button" class="close" data-dismiss="alert">&times;</button>
					<strong>Thank you.</strong> The customer has been booked onto the handheld unit.  View the bookings list to verify or enter another booking below.
					
				</div>
				<?php
							}
						}
					}
				}
				?>

				
				<p class ="lead">Add a booking to the museum audio guide system.</p>
				
				<form method='post' action='?' class="form-horizontal">

					
					<div class="control-group">
						<label for="name" class="control-label">Customer Name: </label>
						<div class="controls">
							<select required id="name" name="name">
								<option value='' disabled="disabled" selected="selected" style='display:none;'>Please Choose</option>
								
									<!--Query the database to populate the dropdown options -->
									<?php
									$result = mysql_query("SELECT customerID, name FROM Customers ORDER BY name ASC");
										while(($data = mysql_fetch_array($result)) !== false)
										{
					    				echo '<option ';
					    				echo 'value = "'.$data["customerID"].'">'.$data["name"]."</option>";
											}
									?>								
								
							</select>
						</div>
					</div>
					
					<div class="control-group">
						<label for="handheld" class="control-label">Handheld Unit: </label>
						<div class="controls">
							<select required id="handheld" name="handheld">
								<option value='' disabled="disabled" selected="selected" style='display:none;'>Please Choose</option>

									<!--Query the database to populate the dropdown options -->
									<?php
									$result = mysql_query("SELECT ID, mac FROM IGEP ORDER BY mac ASC");
										while(($data = mysql_fetch_array($result)) !== false)
										{
					    				echo '<option ';
					    				echo 'value = "'.$data["mac"].'">'.$data["mac"]."</option>";
											}
									?>									
								
							</select>
						</div>
					</div>

					<div class="control-group">
						<label class="control-label" for="authcode" >Create Authorisation Code: </label>
						<div class="controls">
							<input required id="authcode" name="authcode" type="text" placeholder="Enter Authorisation Code"/>
						</div>
					</div>					

					<div class="control-group">
						<div class="controls">
							<button type="submit" class="btn btn-primary" value="submit">Add Booking</button>
						</div>
					</div>
				</form>


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