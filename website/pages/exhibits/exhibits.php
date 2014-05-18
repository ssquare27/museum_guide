<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en">
<head>
	<title>Exhibits | The British Museum</title>
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
		<h1>Exhibits Manager</h1>
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
					<li><a href="/exhibits/add-track/">Add a Track</a></li>    
					<li class="divider"></li>
					<li><a href="/exhibits/delete-track/">Delete a Track</a></li>    
					<li class="divider"></li>
					<li><a href="/exhibits/edit-track/">Edit a Track</a></li>    
					<li class="divider"></li>							
				</ul>
            </div> <!-- /span -->
			<div class="span10">
				<p class ="lead">Make changes to the Exhibits in the Museum Audio Guide System</p>

				<p>Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nunc tristique nisl enim, vel scelerisque magna porta eu. Nam sit amet arcu tincidunt ante posuere tempus. Cras ac hendrerit nisl, vitae molestie tellus. Phasellus mollis sapien vel turpis mollis, at egestas metus gravida. Vivamus ac diam id leo sollicitudin cursus. Pellentesque ac placerat risus, sed vulputate orci. Donec semper a libero sodales tempor. Morbi adipiscing a massa quis sodales. Sed porta facilisis velit, in gravida sapien tincidunt vel. Aliquam non ligula id sem tincidunt facilisis vel in dui. Fusce ut libero rutrum, pretium dui ut, auctor purus. Ut placerat tincidunt ipsum, in accumsan nunc scelerisque eu.</p>
				
				<p>Suspendisse leo lorem, sollicitudin quis ipsum in, accumsan semper augue. Quisque eget mi libero. Suspendisse egestas ligula vel ante sodales, dignissim pellentesque nunc pellentesque. Quisque scelerisque dolor tristique ultricies luctus. In ut pharetra risus. Suspendisse massa magna, tempus ut nibh non, porta ultrices lorem. Pellentesque placerat massa vitae fringilla vulputate. Maecenas sit amet sagittis velit, eget porta massa. Nunc rutrum dolor ut lacinia semper. Etiam luctus sem dui, eu aliquet magna rutrum vitae. Nunc lorem risus, ultrices quis elit vitae, pretium hendrerit neque. Curabitur nec ante et odio faucibus tristique non quis sem. Praesent sit amet accumsan ligula. Praesent molestie molestie est, a vestibulum eros hendrerit fermentum. Curabitur placerat urna ac tortor tincidunt ullamcorper. Vestibulum tempor, nibh a ullamcorper venenatis, nulla magna ultrices erat, sed tincidunt nisl nunc quis nulla.</p> 

				<p>Etiam scelerisque massa lectus, quis pellentesque mi consequat sed. Ut urna justo, pretium non mi quis, pellentesque tristique sapien. Morbi sit amet accumsan ante. Duis rhoncus, nisl quis viverra tempus, nisl libero cursus dolor, pellentesque suscipit nulla tortor vitae metus. Proin at quam tortor. Maecenas luctus ultricies ipsum, in adipiscing ipsum. Praesent ac cursus nisl, nec ultrices felis. Curabitur suscipit nunc ligula, vitae cursus neque tempor nec. Nunc adipiscing augue ut leo ultrices, ac tempus nisl eleifend. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Proin eleifend, tellus ac eleifend accumsan, metus lacus egestas tellus, non elementum ligula odio quis tortor.</p> 

				<p>Aliquam erat volutpat. Aliquam bibendum mauris sit amet metus lobortis pharetra. Vivamus varius at ante in cursus. Pellentesque eu aliquet ligula, ac ullamcorper arcu. In hac habitasse platea dictumst. Nulla a commodo dolor. Duis et ligula tempus, ornare dui eget, sollicitudin velit. Aenean a massa lorem. Sed euismod nisi sit amet consectetur vestibulum. Nulla gravida id lectus nec placerat. Curabitur imperdiet tincidunt felis, sed accumsan dolor porta non. Suspendisse non quam elit.</p> 

				<p>Duis sed quam enim. Donec dapibus pulvinar arcu et viverra. Curabitur justo tellus, scelerisque et tortor at, porttitor tempor augue. Mauris ultrices nibh a pulvinar lobortis. Aenean sollicitudin ligula eu odio consequat auctor. Sed consequat pharetra ligula laoreet dapibus. Phasellus commodo pulvinar nisi, rutrum feugiat turpis auctor a. Quisque placerat magna eu lorem lobortis eleifend. Proin non est vitae arcu dapibus vehicula.</p> 
				
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