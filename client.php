<?
/**
 * @author Marcio Ribeiro (mmr)
 * @created 20/08/2005
 * @version $Id: client.php,v 1.1 2005/08/21 16:35:53 mmr Exp $
 */

$fp = fsockopen("udp://127.0.0.1", 1024, $errno, $errstr);
if (!$fp) {
    exit("Peh!\n");
}
fwrite($fp, $_SERVER['argv'][1]);
fclose($fp);
?>
