<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<head>
	<title>Tracks | The British Museum</title>
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
		<h1>Tracks List</h1>
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
					<li><a href="/exhibits/delete-exhibit/">Delete an Exhibit</a></li>    
					<li class="divider"></li>
					<li><a href="/exhibits/edit-exhibit/">Edit an Exhibit</a></li>    
					<li class="divider"></li>
 					<li class="active"><a href="/exhibits/view-tracks/">View All Tracks</a></li>    
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
				<p class ="lead">Listed below are all audio tracks currently stored for use within the audio guide system.</p>
				
				<?php
				// Define the query.
				$query = "SELECT Audio.trackID, Audio.audioCode, Audio.filePath, Languages.name as language, Expertise.name as expertise, Exhibits.name as exhibit
				FROM Audio
				LEFT JOIN Languages
				ON Audio.language=Languages.code
				LEFT JOIN Expertise
				on Audio.expertise=Expertise.levelID
				LEFT JOIN Exhibits
				on Audio.audioCode=Exhibits.audioCode
				ORDER BY audioCode,language,expertise";
				
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
							<th>Track ID</th>
							<th>Audio Code</th>
							<th>Exhibit Name</th>
							<th>Language</th>
							<th>Expertise Level</th>
							<th>File Location</th>
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
							echo htmlentities(mysql_result($result,$i,"trackID"), ENT_QUOTES);
						echo"</td>";
						
						echo"<td>";
							echo htmlentities(mysql_result($result,$i,"audioCode"), ENT_QUOTES);
						echo"</td>";
						
						echo"<td>";
							echo htmlentities(mysql_result($result,$i,"exhibit"), ENT_QUOTES);
						echo"</td>";

						echo"<td>";
							echo htmlentities(mysql_result($result,$i,"language"), ENT_QUOTES);
						echo"</td>";
						
						echo"<td>";
							echo htmlentities(mysql_result($result,$i,"expertise"), ENT_QUOTES);
						echo"</td>";

						echo"<td>";
							$filepath=htmlentities(mysql_result($result,$i,"filePath"), ENT_QUOTES);
							echo "<a href='$filepath' title='Asset Link'><i class='icon-headphones'></i> $filepath</a>";
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
					echo "<p><strong>Sorry, we don&#8217;t appear to have any audio tracks currently for use with the audio guide system.</strong></p>";
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