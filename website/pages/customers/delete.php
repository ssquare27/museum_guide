<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<head>
	<title>Delete Customer | The British Museum</title>
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
	<?php include $_SERVER['DOCUMENT_ROOT'].'/assets/common/topnavs/customers.php';?>
	
	<?php
	include $_SERVER['DOCUMENT_ROOT'].'/assets/common/database_credentials.php';
	include $_SERVER['DOCUMENT_ROOT'].'/assets/common/database_connect.php';
	?>
	<div class="main-content"><!-- full screen width -->
  <div class="container">
		<h1>Delete a Customer</h1>
		<div class="row">
			<div class="span2">
				<ul class="nav nav-list side-nav">
					<li class="nav-header">Customers Manager</li>
					<li><a href="/customers/view-customers">View All Customers</a></li>    
					<li class="divider"></li>
					<li><a href="/customers/add-customer">Add a Customer</a></li>   
					<li class="divider"></li>
					<li class="active"><a href="/customers/delete-customer">Delete a Customer</a></li>    
					<li class="divider"></li>
					<li><a href="/customers/edit-customer">Edit a Customer</a></li>    
					<li class="divider"></li>                     
				</ul>
            </div> <!-- /span -->
			<div class="span10">
			
				<?php
				if (!empty($_REQUEST['customerID'])) {

					$customerID=$_POST['customerID'];
					
					$getname = "SELECT name FROM Customers WHERE customerID=$customerID";
					$result = mysql_query($getname);
					$name = htmlentities(mysql_result($result,0,"name"), ENT_QUOTES);
					
			
				
					$delete = "DELETE FROM Customers WHERE customerID=$customerID";
					
					mysql_query($delete);
				?>

				
					<div class="alert alert-success">
						<button type="button" class="close" data-dismiss="alert">&times;</button>
						<strong>Thankyou.</strong> Customer <i><?php echo $name; ?></i> has been deleted.  View the customer list below to verify.
					</div>
				<?php
				}
				?>			
			
				<p class ="lead">Listed below are all customers stored in the database.</p>
				<?php
				// Define the query.
				$query = "SELECT Customers.customerID, Customers.name as name, Customers.address, Languages.name as language, Expertise.name as expertise
				FROM Customers
				LEFT JOIN Languages
				ON Customers.language=Languages.code
				LEFT JOIN Expertise
				ON Customers.expertise=Expertise.levelID
				ORDER BY Customers.customerID";
				
				// Run the query.
				$result = mysql_query($query);

				// Count the number of sermons returned
				$num = mysql_numrows($result);

				// Close database connection
				mysql_close();
				
				// Check if there were any results
				if ($num > 0){
				?>
				
				<p>To delete a customer, select the button in the customer row and then click the delete customer button below the table.</p>
				
				<form method='post' action='?'>
					<fieldset>
						<table class='table table-bordered'>
							<thead>
								<tr>
									<th>ID</th>
									<th>Name</th>
									<th>Address</th>
									<th>Language Chosen</th>
									<th>Expertise Level</th>
									<th>Select to Delete</th>
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
									echo htmlentities(mysql_result($result,$i,"customerID"), ENT_QUOTES);
								echo"</td>";
								
								echo"<td>";
									echo htmlentities(mysql_result($result,$i,"name"), ENT_QUOTES);
								echo"</td>";
								
								echo"<td>";
									echo htmlentities(mysql_result($result,$i,"address"), ENT_QUOTES);
								echo"</td>";
								
								echo"<td>";
									echo htmlentities(mysql_result($result,$i,"language"), ENT_QUOTES);
								echo"</td>";
								
								echo"<td>";
									echo htmlentities(mysql_result($result,$i,"expertise"), ENT_QUOTES);
								echo"</td>";
								
								echo"<td>";
									echo'<label class="radio">';
									echo '<input name="customerID" value="'.htmlentities(mysql_result($result,$i,"customerID"), ENT_QUOTES).'" type="radio"  onclick="document.getElementById('."'subutton'".').disabled = false" /> Delete this customer';

									echo"</label>";
								echo"</td>";
							
							echo"</tr>";
								$i++; // increment the count
							} // while loop going through each file returned.
							?>
							<!-- /for each row -->
							</tbody>
						</table>
						<button id="subutton" type="submit" name="delete" class="btn btn-danger" disabled="disabled" value="submit">Delete Selected Customer</button>
					</fieldset>
				</form>
			
				<?php
				} // there were results
				else {
					// there were no results.
					echo "<p><strong>Sorry, we don&#8217;t appear to have any customers.</strong></p>";
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