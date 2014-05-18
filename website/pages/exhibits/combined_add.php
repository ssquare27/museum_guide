<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<head>
	<title>Add Exhibit &#38; Audio | The British Museum</title>
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
		<h1>Add an Exhibit with Audio Tracks</h1>
		<div class="row">
			<div class="span2">
				<ul class="nav nav-list side-nav">
					<li class="nav-header">Exhibits Manager</li>
					<li><a href="/exhibits/view-exhibits/">View All Exhibits</a></li>   
					<li class="divider"></li>
					<li class="active"><a href="/exhibits/add/">Add Exhibit &#38; Audio</a></li>    
					<li class="divider"></li>
					<li><a href="/exhibits/add-exhibit/">Add an Exhibit</a></li>    
					<li class="divider"></li>
					<li><a href="/exhibits/delete-exhibit/">Delete an Exhibit</a></li>    
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
				if (!empty($_REQUEST['name'])) {
					if (!empty($_REQUEST['location'])) {
						if (!empty($_REQUEST['code'])) {
							$name=$_POST['name'];
							$location=$_POST['location'];
							$code=$_POST['code'];
							
							// Remove from post array
							unset($_POST['name']);
							unset($_POST['location']);
							unset($_POST['code']);
						
						
							$insert_exhibit = "INSERT INTO Exhibits ".
									  "(name,location,audioCode) ".
									  "VALUES ( '$name', '$location', '$code' )";
							
							$flag_exhibit_inserted = mysql_query($insert_exhibit);
							
							// Only add to audio table if exhibits insert above succeeded
							if ( $flag_exhibit_inserted ) {
								
								// Loop through all parameters of filepaths for each language and expertise
								foreach ($_POST as $lang_expertise => $filepath) {
								
									$lang_expertise = explode("|", $lang_expertise);
									$language = $lang_expertise[0];
									$expertise = $lang_expertise[1];
									
									$insert_audio = "INSERT INTO Audio ".
										  "(audioCode, expertise, language, filePath) ".
										  "VALUES ( '$code', '$expertise', '$language', '$filepath' )";
										  
									$flag_audio_inserted = mysql_query($insert_audio);
									
									if ($flag_audio_inserted ) {
									}
								
								} // Loop of each language and expertise

							} else {

							  // Exhibit was not added to exhibits table, so dont add tracks to audio table

							} 
				?>

				
				<div class="alert alert-success">
					<button type="button" class="close" data-dismiss="alert">&times;</button>
					<strong>Thank you.</strong> An exhibit called <i><?php echo $name; ?></i> has now been added along with the associated tracks.  View the exhibits and tracks lists to verify or enter another exhibit below.
					
				</div>
				<?php
						}
					}
				}
				?>

				
				<p class ="lead">Add an exhibit and associated tracks to the museum catalogue.</p>
				
				<form method='post' action='?' class="form-horizontal">

					<div class="control-group">
						<label class="control-label" for="name" >Exhibit Name: </label>
						<div class="controls">
							<input required id="name" name="name" type="text" placeholder="Enter Exhibit Name"/>
						</div>
					</div>
					
					<div class="control-group">
						<label class="control-label" for="location" >Exhibit Location: </label>
						<div class="controls">
							<input required id="location" name="location" type="text" placeholder="Enter Exhibit Location"/>
						</div>
					</div>
					
					<div class="control-group">
						<label class="control-label" for="code" >Audio Code: </label>
						<div class="controls">
							<input required id="code" name="code" type="number" maxlength="2" min="1" max="99" placeholder="Enter Audio Code"/>
						</div>
					</div>
					
				<?php
				// Loop through Languages
				
				// Define the query.
				$query = "SELECT * FROM Languages ORDER BY name ASC";
				
				// Run the query.
				$lang_result = mysql_query($query);

				// Count the number of languages returned
				$num_lang = mysql_numrows($lang_result);
			
				// Loop through each language in turn.
				$l=0;
				while ($l < $num_lang) {
					
					
						
					// Loop through Expertise
					
					// Define the query.
					$query = "SELECT * FROM Expertise ORDER BY levelID ASC";
					
					// Run the query.
					$expert_result = mysql_query($query);

					// Count the number of expertise returned
					$num_expert = mysql_numrows($expert_result);
				
					// Loop through each expertise in turn.
					$e=0;
					while ($e < $num_expert) {
						$language_code = htmlentities(mysql_result($lang_result,$l,"code"), ENT_QUOTES);
						$expertise_level = htmlentities(mysql_result($expert_result,$e,"levelID"), ENT_QUOTES);
						$language_name = htmlentities(mysql_result($lang_result,$l,"name"), ENT_QUOTES);
						$expertise_name = htmlentities(mysql_result($expert_result,$e,"name"), ENT_QUOTES);

						
						echo'		

						<div class="control-group">
							<label class="control-label" for="file" >'.$language_name.' '.$expertise_name.' Track File Path: </label>
							<div class="controls">
								<input required id="file" name="'.$language_code.'|'.$expertise_level.'" type="text" placeholder="Enter Track Location"/>
							</div>
						</div>	
						';
					
						
						
						$e++; // increment the expertise count
					} // while loop going through each expertise returned.
					
					
					
					
					
					$l++; // increment the language count
				} // while loop going through each language returned.

				?>
				
				



					

					<div class="control-group">
						<div class="controls">
							<button type="submit" class="btn btn-primary" value="submit">Add Exhibit &#38; Audio</button>
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