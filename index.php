<?
/**
 * @author Marcio Ribeiro (mmr)
 * @created 21/08/2005
 * @version $Id: index.php,v 1.6 2005/08/21 21:51:51 mmr Exp $
 */

define('VEGETA',        '192.168.10.2');
define('LOG_FILE',      'service.log');
define('LOCK_FILE',     'reconnect.lock');
define('URL',           $_SERVER['SCRIPT_NAME']);
define('SERVICE_URL',   'udp://127.0.0.1');
define('SERVICE_PORT',  '666');

if ($_SERVER['REMOTE_ADDR'] != VEGETA) {
    exit();
}

if (isset($_REQUEST['cmd'])) {
    $fp = fsockopen(SERVICE_URL, SERVICE_PORT, $errno, $errstr);
    if (!$fp) {
        exit("Peh!\n");
    }
    fwrite($fp, $_REQUEST['cmd']);
    fclose($fp);
}
?>
<html xmlns='http://www.w3.org/1999/xhtml' xml:lang='pt-br'>
<head>
  <meta http-equiv='pragma' content='no-cache'/>
  <meta http-equiv='content-type' content='text/html; charset=ISO-8859-1'/>
  <link rel='stylesheet' href='css/oldies.css'/>
  <style type='text/css'>
    @import 'css/service.css';
  </style>
  <title>Service</title>
</head>
<body>
<?
if(isset($_REQUEST['log'])) {
?>
    <meta http-equiv='refresh' content='5;url=<?= URL ?>?log=1' />
    <pre id='log'>
<?
    readfile(LOG_FILE);
?>
    </pre>
    <hr />
    <div id='clock'><?= date('D M j G:i:s') ?></div>
    <!--
    <form name='f'>
        <input type='checkbox' name='s' value='1' /> Trava barra de rolagem
    </form>
    -->
    <script type='text/javascript'>
        <!--
        function reload() {
            parent.location = '<?= URL ?>';
        }

        function whatTimeIsIt() {
            var myDate = new Date()
            var aux = myDate.toString();
            document.getElementById('clock').innerHTML = aux;
        }

        function goToEnd() {
            //if (!document.f.s.checked) {
                window.scrollBy(0, 10000);
            //}
        }
        //setInterval('goToEnd()', 1000);
        //setInterval('whatTimeIsIt()', 1000);
        goToEnd();
        //whatTimeIsIt();
        //setTimeOut('reload()', 2);
        // -->
    </script>
<?
} else {
    $isLocked = file_exists(LOCK_FILE);
?>
    <script type='text/javascript'>
        function go(obj, cmd) {
            var f = obj.form;
            f.cmd.value = cmd;
            f.submit();
        }
    </script>
    <h1>Service</h1>
    <hr />
    <div id='main'>
        <div id='log'>
            <h3>Log</h3>
            <iframe src='<?= URL ?>?log=1' width='100%' height='300'></iframe>
        </div>
        <div id='commands'>
            <br /><br />
            <form name='fa' action='<?= URL ?>' method='get'>
                <input type='hidden' name='cmd' value='' />
                <input type='button' value=' Reconectar! '
                    onclick="go(this, 'reconnect')"
                    <?= ($isLocked)?'disabled':''?> />

                <input type='submit' value=' Recarregar página ' />

                <input type='button' value=' Forçar Trava! '
                    onclick="go(this, 'forceLock')" disabled='disabled' />
            </form>
        </div>
    </div>
<?
}
?>
</body>
</html>
