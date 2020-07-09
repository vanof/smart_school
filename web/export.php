<?PHP
function cleanData(&$str)
{
	$str = preg_replace("/\t/", "\\t", $str);
	$str = preg_replace("/\r?\n/", "\\n", $str);
	if(strstr($str, '"')) $str = '"' . str_replace('"', '""', $str) . '"';
	$str = mb_convert_encoding($str, 'UTF-16LE', 'UTF-8');
}

//$xls = new PHPExcel();

// Имя загружаемого файла. 
$filename = date('Y') . "_" .  date('m') . "_" .  date('d') . "_temperatures.xls";

// Подключение к бд
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

mysqli_query($conn, 'SET character_set_database = utf8'); 
mysqli_query($conn, "SET NAMES 'utf8'");
error_reporting(E_ALL); 
ini_set("display_errors", 1);
       
   if( !defined( "ExcelExport" ) ) {
    define( "ExcelExport", 1 );
      class ExportToExcel {
        var $xlsData = ""; 
        var $fileName = ""; 
        var $countRow = 0; 
        var $countCol = 0; 
        var $totalCol = 3;//общее число  колонок в Excel
                //конструктор класса
        function __construct (){
                $this->xlsData = pack( "ssssss", 0x809, 0x08, 0x00,0x10, 0x0, 0x0 );
        }
                // Если число
        function RecNumber( $row, $col, $value ){
                $this->xlsData .= pack( "sssss", 0x0203, 14, $row, $col, 0x00 );
                $this->xlsData .= pack( "d", $value );
                return;
        }
                //Если текст
        function RecText( $row, $col, $value ){
                $len = strlen( $value );
                $this->xlsData .= pack( "s*", 0x0204, 8 + $len, $row, $col, 0x00, $len);
                $this->xlsData .= $value;
                return;
        }
                // Вставляем число
        function InsertNumber( $value ){
                if ( $this->countCol == $this->totalCol ) {
                        $this->countCol = 0;
                        $this->countRow++;
                }
                $this->RecNumber( $this->countRow, $this->countCol, $value );
                $this->countCol++;
                return;
        }
                // Вставляем текст
        function InsertText( $value ){
                if ( $this->countCol == $this->totalCol ) {
                        $this->countCol = 0;
                        $this->countRow++;
        }
                $this->RecText( $this->countRow, $this->countCol, $value );
                $this->countCol++;
                return;
        }
                // Переход на новую строку
        function GoNewLine(){
                $this->countCol = 0;
                $this->countRow++;
                return;
                }
                //Конец данных
        function EndData(){
                $this->xlsData .= pack( "ss", 0x0A, 0x00 );
                return;
        }
                // Сохраняем файл
        function SaveFile( $fileName ){
                $this->fileName = $fileName;
                $this->SendFile();
        }
                // Отправляем файл
        function SendFile(){
                $this->EndData();
				header("Content-Type: text/csv; charset=UTF-16LE");
                header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
                header ( "Cache-Control: no-store, no-cache, must-revalidate" );
                header ( "Pragma: no-cache" );
                header ( "Content-type: application/x-msexcel" );
                header ( "Content-Disposition: attachment; fileName=$this->fileName.xls" );
                print $this->xlsData;
         }
        } 
   }
      //if(isset($_GET['id'])) {
                //фильтруем данные
   //$id = mysql_real_escape_string(stripslashes(trim(htmlspecialchars($_GET['id'],ENT_QUOTES))));
                $filename = date('Y') . "_" .  date('m') . "_" .  date('d') . "_temperatures.xls";
                $excel = new ExportToExcel(); // создаем экземпляр класса
                $sql="SELECT * FROM  Sensor";//запрос к базе
				//mysqli_query($conn, "set names cp1251");
				//mysqli_query($conn, 'SET character_set_database = cp1251_general_ci') or die('Query1 failed!');
				mysqli_query($conn, "SET NAMES 'cp1251'") or die('Query2 failed!');
                $rez=mysqli_query($conn, $sql);
                $excel->InsertText('Time');
                $excel->InsertText('Temperature');
                $excel->InsertText('Humidity');
                $excel->GoNewLine();
        While($row=mysqli_fetch_assoc($rez)){
                //$excel->InsertNumber($row['reading_time']);
				$excel->InsertText($row['reading_time']);
                $excel->InsertText($row['value1']);
                $excel->InsertText($row['value2']);
                $excel->GoNewLine();
        }
        $excel->SaveFile($filename);
   //} 
?>
