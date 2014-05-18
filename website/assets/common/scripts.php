	<script type="text/javascript" src="http://code.jquery.com/jquery.min.js"></script>
	<!-- Fallback to local jQuery in unlikely event that CDN is down -->
	<script type="text/javascript">
		if (typeof jQuery == 'undefined') {
		    document.write(unescape("%3Cscript src='/assets/js/jquery.min.js' type='text/javascript'%3E%3C/script%3E"));
		}
	</script>

	<script type="text/javascript" src="//netdna.bootstrapcdn.com/twitter-bootstrap/2.3.2/js/bootstrap.min.js"></script>
	<!-- Fallback to local bootstrap js in unlikely event that CDN is down -->
	<script type="text/javascript">
		if(typeof($.fn.modal) === 'undefined') {document.write('\x3Cscript type="text/javascript" src="/assets/js/bootstrap.min.js">\x3C\/script>')}
	</script>

	<!--[if lte IE 6]>
		<script type="text/javascript" src="/assets/js/bootstrap-ie.js"></script>
	<![endif]-->

	<!-- Check browsers that should have used CDN for CSS; fallback to local if no CSS file -->
	<!--[if !IE]><!-->
		<script type="text/javascript">$(document).ready(function() {
			var bodyColor = $('body').css("color"); if(bodyColor != 'rgb(51, 51, 51)') {$("head").prepend("\x3Clink rel='stylesheet' href='/assets/css/bootstrap.min.css' type='text/css' media='screen'/>\x3Clink rel='stylesheet' href='/assets/css/main.css' type='text/css' media='screen'/>");}
			});
		</script>
	<!--<![endif]-->

	<script type="text/javascript">
		$(function() {
		    if (window.PIE) {
		        $('.img-rounded').each(function() {
		            PIE.attach(this);
		        });
		    }
		});
	</script>
	<script type="text/javascript">
		$(function() {
		    if (window.PIE) {
		        $('.img-circle').each(function() {
		            PIE.attach(this);
		        });
		    }
		});
	</script>
	<script type="text/javascript">
		$(function() {
		    if (window.PIE) {
		        $('.carousel-control').each(function() {
		            PIE.attach(this);
		        });
		    }
		});
	</script>
	
	<script type="text/javascript" src="//cdnjs.cloudflare.com/ajax/libs/retina.js/1.0.1/retina.min.js" async="async"></script>
	
	<script type="text/javascript">
	 function downloadJSAtOnload() {
		 var element = document.createElement("script");
		 element.src = "http://code.jquery.com/jquery-migrate-1.2.1.min.js";
		 document.body.appendChild(element);
	 }
	 if (window.addEventListener)
		window.addEventListener("load", downloadJSAtOnload, false);
	 else if (window.attachEvent)
		window.attachEvent("onload", downloadJSAtOnload);
	 else window.onload = downloadJSAtOnload;
	</script>	

	<script type="text/javascript" src="//cdnjs.cloudflare.com/ajax/libs/bootstrap-lightbox/0.6.1/bootstrap-lightbox.min.js" async="async"></script>

	