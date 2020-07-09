<?php

$servername = "localhost";

// REPLACE with your Database name
$dbname = "h005344445_smart";
// REPLACE with Database user
$username = "h005344445_smart";
// REPLACE with Database user password
$password = "Nevermore32";

// Create connection 
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 

$sql = "SELECT id, value1, value2, value3, value4, calibration, reading_time FROM Sensor order by reading_time desc limit 20";

$result = $conn->query($sql);

while ($data = $result->fetch_assoc()){
    $sensor_data[] =$data;
}

$readings_time = array_column($sensor_data, 'reading_time');

// ******* Uncomment to convert readings time array to your timezone ********
/*$i = 0;
foreach ($readings_time as $reading){
    // Uncomment to set timezone to - 1 hour (you can change 1 to any number)
    $readings_time[$i] = date("Y-m-d H:i:s", strtotime("$reading - 1 hours"));
    // Uncomment to set timezone to + 4 hours (you can change 4 to any number)
    //$readings_time[$i] = date("Y-m-d H:i:s", strtotime("$reading + 4 hours"));
    $i += 1;
}*/

$value1 = json_encode(array_reverse(array_column($sensor_data, 'value1')), JSON_NUMERIC_CHECK);
$value2 = json_encode(array_reverse(array_column($sensor_data, 'value2')), JSON_NUMERIC_CHECK);
$value3 = json_encode(array_reverse(array_column($sensor_data, 'value3')), JSON_NUMERIC_CHECK);
$value4 = json_encode(array_reverse(array_column($sensor_data, 'value4')), JSON_NUMERIC_CHECK);
$calibration = json_encode(array_reverse(array_column($sensor_data, 'calibration')), JSON_NUMERIC_CHECK);
$reading_time = json_encode(array_reverse($readings_time), JSON_NUMERIC_CHECK);

$value_1_last =  end(array_reverse(array_column($sensor_data, 'value1')));
$value_2_last =  end(array_reverse(array_column($sensor_data, 'value2')));
$value_3_last =  end(array_reverse(array_column($sensor_data, 'value3')));
$value_4_last =  end(array_reverse(array_column($sensor_data, 'value4')));
$calibration_last =  end(array_reverse(array_column($sensor_data, 'calibration')));
$reading_time_last =  end(array_reverse($readings_time));

/*echo $value1;
echo $value2;
echo $value3;
echo $reading_time;*/

$result->free();
$conn->close();
?>

<!DOCTYPE html>
<html lang="ru">

<meta name="viewport" content="width=device-width, initial-scale=1">
  <!--  JavaScript -->
  <script src="https://code.highcharts.com/highcharts.js"></script>
   <!-- Optional JavaScript -->
    <!-- jQuery first, then Popper.js, then Bootstrap JS
   <script src="https://code.jquery.com/jquery-3.5.1.slim.min.js" integrity="sha384-DfXdz2htPH0lsSSs5nCTpuj/zy4C+OGpamoFVy38MVBnE+IbbVYUew+OrCXaRkfj" crossorigin="anonymous"></script>
   <script src="https://cdn.jsdelivr.net/npm/popper.js@1.16.0/dist/umd/popper.min.js" integrity="sha384-Q6E9RHvbIyZFJoft+2mJbHaEWldlvI9IOYy5n3zV9zzTtmI3UksdQRVvoxMfooAo" crossorigin="anonymous"></script>
   <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.5.0/js/bootstrap.min.js" integrity="sha384-OgVRvuATP1z7JjHLkuOU7Xw704+h835Lr+6QL9UvYjZE3Ipu6Tp75j7Bh/kR0JKI" crossorigin="anonymous"></script>
 -->

  <style>
    body {
     <!-- min-width: 310px; -->
    	max-width: 1920px;
    	height: 687px;
      margin: 0 auto;
    }

	.img__container {
	  position: relative;
	  max-width: 100%;
	  margin: 20px auto 30px auto;
	}

	.img__container>img {
	  display: inline-block;
	  width: 1280px;
      height: 500px;
	  border-radius: 4px;
	  box-shadow: 0 0 6px #9E9E9E;
	  border: 4px solid #fff;
	}

	.img__description {
	  position: absolute;
	  left: 0;
	  right: 0;
	  bottom: 0;
	  padding: 15px;
	  background-color: rgba(255, 255, 255, 0.8);
	  border-bottom-left-radius: 4px;
	  border-bottom-right-radius: 4px;
	}

	.img__header {
	  font-weight: bold;
	  font-size: 1.25em;
	}    
		
  </style>
  
  	<head>
    <!-- Required meta tags  	  
	max-width: 100%;
	height: auto; 
	  -->
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">

    <!-- Bootstrap CSS -->
    <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.0/css/bootstrap.min.css" integrity="sha384-9aIt2nRpC12Uk9gS9baDl411NQApFmC26EwAOH8WgZl5MYYxFfc+NcPb1dKGj7Sk" crossorigin="anonymous">


	<!--<link href="temp/style.css" rel="stylesheet"> -->
	
    <title>Система измерений ГБОУ "Школа №1561" г. Москва</title>
	
	 <link rel="shortcut icon" href="img/1561.ico" type="image/x-icon">
	
  </head>
  
  <body>
	<center>
    <h2>Система измерений ГБОУ "Школа №1561" г. Москва</h2>
	<h4>Частота измерений - 30 минут. Последний замер произведен: <?php echo $reading_time_last; ?> <br></h4>
	</center>
	<div class="img__container">
	  <img src="img/plan2.jpg" class="image" alt="План школы">
		 <div style="width: 180px; height:80px; position:absolute; top:31%; left:3.125%; border: 5px solid #6699ff;">
		 <center>
		 Кабинет №306 <br>	
		 Температура: <?php echo $value_1_last; ?> ℃ <br>
		 Влажность: <?php echo $value_2_last; ?> % <br>
		 </center>
		 </div>
		 <div style="width: 180px; height:80px; position:absolute; top:61%; left:4.125%; border: 5px solid #6699ff;">
		 <center>
		 Кабинет №307 <br>	
		 Температура:  <br>
		 Влажность:  <br>
		 </center>
		 </div>
		<div style="width: 180px; height:80px; position:absolute; top:31%; right:6%; border: 5px solid #6699ff;">
		 <center>
		 Кабинет №314 <br>	
		 Температура: <?php echo $value_3_last; ?> ℃ <br>
		 Влажность: <?php echo $value_4_last; ?> % <br>
		 </center>
		 </div>
	</div>
	
	
	<center>
	<button  type="submit" class="btn btn-primary" 
	onClick="document.location.href='/export.php'">Экспорт в Excel
	</button>	
	</center>

    <div id="chart-temperature" class="container"></div>
    <div id="chart-humidity" class="container"></div>
	
	<!--
	<div class="col">Точность: <select id="intervalMinutes">
		<option value="15">15 мин</option> 
		<option value="30">30 мин</option>
		<option value="60">1 час</option>
	</select>
	</div>
	-->
	
<script>
var value1 = <?php echo $value1; ?>;
var value2 = <?php echo $value2; ?>;
var value3 = <?php echo $value3; ?>;
var value4 = <?php echo $value4; ?>;
var calibration = <?php echo $calibration; ?>;
var reading_time = <?php echo $reading_time; ?>;


var chartT = new Highcharts.Chart({
  chart:{ renderTo : 'chart-temperature' },
  title: { text: 'Температура, точность +-1 градус Цельсия' },
  series: [
  {
	name: '306',
    showInLegend: true,
    data: value1,
  },
  {
	name: 'Калибровка',
    showInLegend: true,
    data: calibration,
	color: '#FF0000'
  },
  {
	name: '314',
    showInLegend: true,
    data: value3
  }
  ],
  plotOptions: {
    line: { animation: false,
      dataLabels: { enabled: true }
    },
    series: { color: '#059e64' }
  },
  xAxis: { 
    type: 'datetime',
    categories: reading_time
  },
  yAxis: {
    title: { text: 'Temperature (Celsius)' }
    //title: { text: 'Temperature (Fahrenheit)' }
  },
  credits: { enabled: false }
});

var chartH = new Highcharts.Chart({
  chart:{ renderTo:'chart-humidity' },
  title: { text: 'Влажность, точность +-10%' },
 series: [
  {
	name: '306',
    showInLegend: true,
    data: value2,
	color: '#FF0000'
  },
  {
	name: '314',
    showInLegend: true,
    data: value4
  }
  ],
  plotOptions: {
    line: { animation: false,
      dataLabels: { enabled: true }
    },
	    series: { color: '#059e8a' }
  },
  xAxis: {
    type: 'datetime',
    //dateTimeLabelFormats: { second: '%H:%M:%S' },
    categories: reading_time
  },
  yAxis: {
    title: { text: 'Humidity (%)' }
  },
  credits: { enabled: false }
});

</script>
</body>
</html>