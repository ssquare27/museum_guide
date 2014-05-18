<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<head>
	<title>Add Track | The British Museum</title>
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
		<h1>Add a Track</h1>
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
 					<li><a href="/exhibits/view-tracks/">View All Tracks</a></li>    
					<li class="divider"></li>
					<li class="active"><a href="/exhibits/add-track/">Add a Track</a></li>    
					<li class="divider"></li>
					<li><a href="/exhibits/delete-track/">Delete a Track</a></li>    
					<li class="divider"></li>
					<li><a href="/exhibits/edit-track/">Edit a Track</a></li>    
					<li class="divider"></li>							
				</ul>
            </div> <!-- /span -->
			<div class="span10">
			
				<?php
				if (!empty($_REQUEST['code'])) {
					if (!empty($_REQUEST['language'])) {
						if (!empty($_REQUEST['expertise'])) {
							if (!empty($_REQUEST['location'])) {
								$code=$_POST['code'];
								$location=$_POST['location'];
								$language=$_POST['language'];
								$expertise=$_POST['expertise'];
							
							
								$insert = "INSERT INTO Audio ".
										  "(audioCode, expertise, language, filePath) ".
										  "VALUES ( '$code', '$expertise', '$language', '$location' )";
								
								mysql_query($insert);
				?>

				
				<div class="alert alert-success">
					<button type="button" class="close" data-dismiss="alert">&times;</button>
					<strong>Thank you.</strong> The track has now been added.  View the track list to verify or enter another track below.
					
				</div>
				<?php
							}
						}
					}
				}
				?>

				
				<p class ="lead">Add a track for use within the audio guide system.</p>
				
				<form method='post' action='?' class="form-horizontal">

								
					<div class="control-group">
						<label class="control-label" for="code" >Audio Code: </label>
						<div class="controls">
							<input required id="code" name="code" type="number" maxlength="2" min="1" max="99" placeholder="Enter Audio Code"/>
						</div>
					</div>
					
					<div class="control-group">
						<label for="language" class="control-label">Language: </label>
						<div class="controls">
							<select required id="language" name="language">
								<option value='' disabled="disabled" selected="selected" style='display:none;'>Please Choose</option>
								
									<!--Query the database to populate the dropdown options -->
									<?php
									$result = mysql_query("SELECT code, name FROM Languages ORDER BY name ASC");
										while(($data = mysql_fetch_array($result)) !== false)
										{
					    				echo '<option ';
					    				echo 'value = "'.$data["code"].'">'.$data["name"]."</option>";
											}
									?>								
								
							</select>
						</div>
					</div>
					
					<div class="control-group">
						<label for="expertise" class="control-label">Expertise Level: </label>
						<div class="controls">
							<select required id="expertise" name="expertise">
								<option value='' disabled="disabled" selected="selected" style='display:none;'>Please Choose</option>

									<!--Query the database to populate the dropdown options -->
									<?php
									$result = mysql_query("SELECT levelID, name FROM Expertise ORDER BY levelID ASC");
										while(($data = mysql_fetch_array($result)) !== false)
										{
					    				echo '<option ';
					    				echo 'value = "'.$data["levelID"].'">'.$data["name"]."</option>";
											}
									?>									
								
							</select>
						</div>
					</div>
					
					<div class="control-group">
						<label class="control-label" for="location" >Track File Path: </label>
						<div class="controls">
							<input required id="location" name="location" type="text" placeholder="Enter Track Location"/>
						</div>
					</div>					

					<div class="control-group">
						<div class="controls">
							<button type="submit" class="btn btn-primary" value="submit">Add Track</button>
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