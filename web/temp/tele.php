<?
ini_set('error_reporting', E_ALL);
ini_set('display_errors', 1);
ini_set('display_startup_errors', 1);

// сюда нужно вписать токен вашего бота
define('TELEGRAM_TOKEN', '******:************');

// сюда нужно вписать ваш внутренний айдишник
define('TELEGRAM_CHATID', '**********');

//echo message_to_telegram('Данные!');

function message_to_telegram($text) {
    $ch = curl_init();
	$url = "https://api.telegram.org/bot724027773:AAGrzdVNYTLdYwD9TjNZv3942ZcEgsv5PnE/sendMessage?chat_id=**********&text=".$text; // где XXXXX - ваши значения

	$prxy       = '162.144.68.41:56505'; // адрес:порт прокси http://spys.one/proxys/US/
	$prxy_auth = 'auth_user:auth_pass';       // логин:пароль для аутентификации
	curl_setopt_array ($ch, array(CURLOPT_URL => $url, CURLOPT_RETURNTRANSFER => true)); 
/********************* Код для подключения к прокси *********************/


    curl_setopt($ch, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS5);  // тип прокси 
    curl_setopt($ch, CURLOPT_PROXY,  $prxy);                 // ip, port прокси
    curl_setopt($ch, CURLOPT_PROXYUSERPWD, $prxy_auth);  // авторизация на прокси
    curl_setopt($ch, CURLOPT_HEADER, false);                // отключение передачи заголовков в запросе 
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);            // возврат результата в качестве строки
    curl_setopt($ch, CURLOPT_POST, 1);                      // использование простого HTTP POST
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);        // отмена проверки сертификата удаленным сервером
	curl_setopt($ch, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
	// Параметры ниже подходят, если у вас PHP свежей версии
	//curl_setopt($ch, CURLOPT_RESOLVE, array("api.telegram.org:443:146.185.158.29"));
    //curl_setopt($ch, CURLOPT_DNS_SERVERS, '8.8.8.8,8.8.4.4');  // тип прокси 
/***********************************************************************/
    $response = curl_exec($ch);
	if ($response === FALSE) {	 
		$response = "cURL Error: " . curl_error($ch);	 
	}
	$info = curl_getinfo($ch); 
	//$response .= '<br/>Took ' . $info['total_time'] . ' seconds for url ' . $info['url']."<br/>";
	curl_close($ch); 
	return $response;
}

?>