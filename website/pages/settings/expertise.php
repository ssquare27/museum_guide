<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<head>
	<title>Expertise Levels | The British Museum</title>
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
	<?php include $_SERVER['DOCUMENT_ROOT'].'/assets/common/topnavs/settings.php';?>
	
	<?php
	include $_SERVER['DOCUMENT_ROOT'].'/assets/common/database_credentials.php';
	include $_SERVER['DOCUMENT_ROOT'].'/assets/common/database_connect.php';
	?>
	<div class="main-content"><!-- full screen width -->
  <div class="container">
		<h1>Expertise Levels</h1>
		<div class="row">
			<div class="span2">
				<ul class="nav nav-list side-nav">
					<li class="nav-header">Settings Manager</li>
					<li><a href="/settings/languages/">Languages</a></li>   
					<li class="divider"></li>
					<li  class="active"><a href="/settings/expertise/">Expertise Levels</a></li>    
					<li class="divider"></li>
					<li><a href="/settings/handheld-units/">Handheld Units</a></li>   
					<li class="divider"></li>					
				</ul>
            </div> <!-- /span -->
			<div class="span10">
				<?php
				if (!empty($_REQUEST['name'])) {
					$name=$_POST['name'];
					
					
					$insert = "INSERT INTO Expertise ".
							  "(name) ".
							  "VALUES ( '$name' )";
					
					mysql_query($insert);
				?>

				
				<div class="alert alert-success">
					<button type="button" class="close" data-dismiss="alert">&times;</button>
					<strong>Thank you.</strong> A expertise level called <i><?php echo $name; ?></i> has now been created.  View the expertise levels list to verify or enter another expertise level below.
					
				</div>
				<?php
				}
				?>

				
				<p class ="lead">Add an expertise level for use with the museum audio guide.</p>
				
				<form method='post' action='?' class="form-horizontal">

					<div class="control-group">
						<label class="control-label" for="name" >Level Name: </label>
						<div class="controls">
							<input required id="name" name="name" type="text" placeholder="Enter Expertise Name"/>
						</div>
					</div>

					<div class="control-group">
						<div class="controls">
							<button type="submit" class="btn btn-primary" value="submit">Add Level</button>
						</div>
					</div>
				</form>

				
				<hr/>
				

				<p class ="lead">Listed below are all expertise levels currently available for use with the museum audio guide.</p>

				<?php
				// Define the query.
				$query = "SELECT * FROM Expertise ORDER BY levelID ASC";
				
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
							<th>Level ID</th>
							<th>Level Name</th>
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
							echo htmlentities(mysql_result($result,$i,"levelID"), ENT_QUOTES);
						echo"</td>";
						
						echo"<td>";
							echo htmlentities(mysql_result($result,$i,"name"), ENT_QUOTES);
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
					echo "<p><strong>Sorry, we don&#8217;t appear to have any expertise levels currently available.</strong></p>";
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