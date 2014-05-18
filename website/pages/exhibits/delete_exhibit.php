<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<head>
	<title>Delete Exhibit | The British Museum</title>
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
	<?php include $_SERVER['DOCUMENT_ROOT'].'/assets/common/topnavs/exhibits.php';?>
	
	<?php
	include $_SERVER['DOCUMENT_ROOT'].'/assets/common/database_credentials.php';
	include $_SERVER['DOCUMENT_ROOT'].'/assets/common/database_connect.php';
	?>
	<div class="main-content"><!-- full screen width -->
  <div class="container">
		<h1>Delete an Exhibit</h1>
		<div class="row">
			<div class="span2">
				<ul class="nav nav-list side-nav">
					<li class="nav-header">Exhibits Manager</li>
					<li><a href="/exhibits/view-exhibits/">View All Exhibits</a></li>   
					<li class="divider"></li>
					<li><a href="/exhibits/add/">Add Exhibit &#38; Audio</a></li>    
					<li class="divider"></li>
					<li><a href="/exhibits/add-exhibit/">Add an Exhibit</a></li>    
					<li class="divider"></li>
					<li class="active"><a href="/exhibits/delete-exhibit/">Delete an Exhibit</a></li>    
					<li class="divider"></li>
					<li><a href="/exhibits/edit-exhibit/">Edit an Exhibit</a></li>    
					<li class="divider"></li>
 					<li><a href="/exhibits/view-tracks/">View All Tracks</a></li>    
					<li class="divider"></li>
					<li><a href="/exhibits/add-track/">Add a Track</a></li>    
					<li class="divider"></li>
					<li><a href="/exhibits/delete-track/">Delete a Track</a></li>    
					<li class="divider"></li>
					<li><a href="/exhibits/edit-track/">Edit a Track</a></li>    
					<li class="divider"></li>							
				</ul>
            </div> <!-- /span -->
			<div class="span10">
			
			
				<?php
				if (!empty($_REQUEST['exhibitID'])) {

					$exhibitID=$_POST['exhibitID'];
					
					$getname = "SELECT name FROM Exhibits WHERE exhibitID=$exhibitID";
					$result = mysql_query($getname);
					$name = htmlentities(mysql_result($result,0,"name"), ENT_QUOTES);
					
			
				
					$delete = "DELETE FROM Exhibits WHERE exhibitID=$exhibitID";
					
					mysql_query($delete);
				?>

				
					<div class="alert alert-success">
						<button type="button" class="close" data-dismiss="alert">&times;</button>
						<strong>Thankyou.</strong> Exhibit <i><?php echo $name; ?></i> has been deleted.  View the exhibit list below to verify.
					</div>
				<?php
				}
				?>		

				
				<p class ="lead">Listed below are all exhibits currently housed within the museum.</p>
				
				
				
				<?php
				// Define the query.
				$query = "SELECT * FROM Exhibits ORDER BY exhibitID ASC";
				
				// Run the query.
				$result = mysql_query($query);

				// Count the number of sermons returned
				$num = mysql_numrows($result);

				// Close database connection
				mysql_close();
				
				// Check if there were any results
				if ($num > 0){
				?>
				
				<p>To delete an exhibit, select the button in the exhibit row and then click the delete exhibit button below the table.</p>
				
				<form method='post' action='?'>
					<fieldset>				
						<table class='table table-bordered'>
							<thead>
								<tr>
									<th>Exhibit ID</th>
									<th>Exhibit Name</th>
									<th>Location</th>
									<th>Audio Code</th>
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
									echo htmlentities(mysql_result($result,$i,"exhibitID"), ENT_QUOTES);
								echo"</td>";
								
								echo"<td>";
									echo htmlentities(mysql_result($result,$i,"name"), ENT_QUOTES);
								echo"</td>";
								
								echo"<td>";
									echo htmlentities(mysql_result($result,$i,"location"), ENT_QUOTES);
								echo"</td>";

								echo"<td>";
									echo htmlentities(mysql_result($result,$i,"audioCode"), ENT_QUOTES);
								echo"</td>";

								echo"<td>";
									echo'<label class="radio">';
									echo '<input name="exhibitID" value="'.htmlentities(mysql_result($result,$i,"exhibitID"), ENT_QUOTES).'" type="radio"  onclick="document.getElementById('."'subutton'".').disabled = false" /> Delete this exhibit';

									echo"</label>";
								echo"</td>";								
							
							echo"</tr>";
								$i++; // increment the count
							} // while loop going through each file returned.
							?>
							<!-- /for each row -->
							</tbody>
						</table>
						<button id="subutton" type="submit" name="delete" class="btn btn-danger" disabled="disabled" value="submit">Delete Selected Exhibit</button>
					</fieldset>
				</form>
				
				<?php
				} // there were results
				else {
					// there were no results.
					echo "<p><strong>Sorry, we don&#8217;t appear to have any exhibits currently housed within the museum.</strong></p>";
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