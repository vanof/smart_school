<!DOCTYPE html>
<html>

<!-- служебная часть -->
<head>
  <!-- заголовок страницы -->
  <title>Адаптивная вёрстка на Бутстрапе</title>
  <!-- настраиваем служебную информацию для браузеров -->
  <meta charset="utf-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css" integrity="sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T" crossorigin="anonymous">
  <!-- задаём CSS-стили прямо здесь же, чтобы всё было в одном файле -->
  <style type="text/css">
    img{
          max-width: 100%;
       } 
  </style>
<!-- закрываем служебную часть страницы    <div class="col-12 col-sm-12 col-md-8 col-lg-8 col-xl-8">-->
</head>
<body>
  <div class="container" >
    <div class="row">
       <div class="col-12">
        <h1>тратата</h1>
      </div>
     </div>
  </div>

<div id="carouselExampleCaptions" class="carousel slide" data-ride="carousel">
  <ol class="carousel-indicators">
    <li data-target="#carouselExampleCaptions" data-slide-to="0" class="active"></li>
    <li data-target="#carouselExampleCaptions" data-slide-to="1"></li>
    <li data-target="#carouselExampleCaptions" data-slide-to="2"></li>
  </ol>
  <div class="carousel-inner">
    <div class="carousel-item active">
      <img src="img/plan-1.jpg" class="d-block w-100" alt="...">

      <div class="carousel-caption d-none d-md-block">
		<div class="p-3 mb-2 bg-white text-dark">Первый этаж</div>
        <!--<p>Nulla vitae elit libero, a pharetra augue mollis interdum.</p> -->
      </div>
    </div>
    <div class="carousel-item">
      <img src="img/plan-2.jpg" class="d-block w-100" alt="...">
      <div class="carousel-caption d-none d-md-block">
		<div class="p-3 mb-2 bg-white text-dark">Второй этаж</div>
      </div>
    </div>
    <div class="carousel-item">
      <img src="img/plan-3.jpg" class="d-block w-100" alt="...">
      <div class="carousel-caption d-none d-md-block">
		<div class="p-3 mb-2 bg-white text-dark"><h4>Третий этаж</h4></div>
      </div>
    </div>
  </div>

  <a class="carousel-control-prev" href="#carouselExampleCaptions" role="button" data-slide="prev">
    <span class="carousel-control-prev-icon" aria-hidden="true"></span>
    <span class="sr-only">Previous</span>
  </a>
  <a class="carousel-control-next" href="#carouselExampleCaptions" role="button" data-slide="next">
    <span class="carousel-control-next-icon" aria-hidden="true"></span>
    <span class="sr-only">Next</span>
  </a>
</div>



  <div class="container" >
	<div class="row">
	  <div class="col-xs-12 col-sm-6 col-md-8">.col-xs-12 .col-sm-6 .col-md-8</div>
	  <div class="col-xs-6 col-md-4">.col-xs-6 .col-md-4</div>
	</div>
	
	<div class="row">
	  <div class="col-xs-6 col-sm-4">.col-xs-6 .col-sm-4</div>
	  <div class="col-xs-6 col-sm-4">.col-xs-6 .col-sm-4
	  <img src="img/plan2.jpg" class="image" alt="План школы">
	  </div>
	  <!-- Дополнительно: очистить XS седла если их содержание не совпадает в высоту -->
	  <div class="clearfix hidden-sm-up"></div>
	  <div class="col-xs-6 col-sm-4">.col-xs-6 .col-sm-4</div>
	</div>
  </div>
  
  
   <script src="https://code.jquery.com/jquery-3.5.1.slim.min.js" integrity="sha384-DfXdz2htPH0lsSSs5nCTpuj/zy4C+OGpamoFVy38MVBnE+IbbVYUew+OrCXaRkfj" crossorigin="anonymous"></script>
   <script src="https://cdn.jsdelivr.net/npm/popper.js@1.16.0/dist/umd/popper.min.js" integrity="sha384-Q6E9RHvbIyZFJoft+2mJbHaEWldlvI9IOYy5n3zV9zzTtmI3UksdQRVvoxMfooAo" crossorigin="anonymous"></script>
   <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.0/js/bootstrap.min.js" integrity="sha384-OgVRvuATP1z7JjHLkuOU7Xw704+h835Lr+6QL9UvYjZE3Ipu6Tp75j7Bh/kR0JKI" crossorigin="anonymous"></script>

  
</body>
<!-- конец всей страницы -->

</html> 