<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<head>
	<title>Add Exhibit | The British Museum</title>
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
		<h1>Add an Exhibit</h1>
		<div class="row">
			<div class="span2">
				<ul class="nav nav-list side-nav">
					<li class="nav-header">Exhibits Manager</li>
					<li><a href="/exhibits/view-exhibits/">View All Exhibits</a></li>   
					<li class="divider"></li>
					<li><a href="/exhibits/add/">Add Exhibit &#38; Audio</a></li>    
					<li class="divider"></li>
					<li class="active"><a href="/exhibits/add-exhibit/">Add an Exhibit</a></li>    
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
						
						
							$insert = "INSERT INTO Exhibits ".
									  "(name,location,audioCode) ".
									  "VALUES ( '$name', '$location', '$code' )";
							
							mysql_query($insert);
				?>

				
				<div class="alert alert-success">
					<button type="button" class="close" data-dismiss="alert">&times;</button>
					<strong>Thank you.</strong> An exhibit called <i><?php echo $name; ?></i> has now been added.  View the exhibit list to verify or enter another exhibit below.
					
				</div>
				<?php
						}
					}
				}
				?>

				
				<p class ="lead">Add an exhibit to the museum catalogue.</p>
				
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

					<div class="control-group">
						<div class="controls">
							<button type="submit" class="btn btn-primary" value="submit">Add Exhibit</button>
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