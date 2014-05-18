<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<head>
	<title>Add Customer | The British Museum</title>
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
		<h1>Add a Customer</h1>
		<div class="row">
			<div class="span2">
				<ul class="nav nav-list side-nav">
					<li class="nav-header">Customers Manager</li>
					<li><a href="/customers/view-customers">View All Customers</a></li>    
					<li class="divider"></li>
					<li class="active"><a href="/customers/add-customer">Add a Customer</a></li>   
					<li class="divider"></li>
					<li><a href="/customers/delete-customer">Delete a Customer</a></li>    
					<li class="divider"></li>
					<li><a href="/customers/edit-customer">Edit a Customer</a></li>    
					<li class="divider"></li>                     
				</ul>
            </div> <!-- /span -->
			<div class="span10">
				<?php
				if (!empty($_REQUEST['name'])) {
					if (!empty($_REQUEST['address'])) {
						if (!empty($_REQUEST['language'])) {
							if (!empty($_REQUEST['expertise'])) {
								$name=$_POST['name'];
								$address=$_POST['address'];
								$language=$_POST['language'];
								$expertise=$_POST['expertise'];
							
							
								$insert = "INSERT INTO Customers ".
										  "(name,address,language,expertise) ".
										  "VALUES ( '$name', '$address', '$language', '$expertise' )";
								
								mysql_query($insert);
				?>

				
				<div class="alert alert-success">
					<button type="button" class="close" data-dismiss="alert">&times;</button>
					<strong>Thank you.</strong> A customer called <i><?php echo $name; ?></i> has now been added.  View the customers list to verify or enter another customer below.
					
				</div>
				<?php
							}
						}
					}
				}
				?>

				
				<p class ="lead">Add a customer to the museum audio guide system.</p>
				
				<form method='post' action='?' class="form-horizontal">

					<div class="control-group">
						<label class="control-label" for="name" >Customer Name: </label>
						<div class="controls">
							<input required id="name" name="name" type="text" placeholder="Enter Customer Name"/>
						</div>
					</div>
					
					<div class="control-group">
						<label class="control-label" for="address" >Customer Address: </label>
						<div class="controls">
							<input required id="address" name="address" type="text" placeholder="Enter Customer Address"/>
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
						<div class="controls">
							<button type="submit" class="btn btn-primary" value="submit">Add Customer</button>
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