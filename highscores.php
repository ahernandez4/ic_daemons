<?php
if(isset($_GET['newtime'])) {
    $highscorefile = fopen("highscores.txt","w"); or die("unable to open file");
    $newtime = $_GET['newtime'];
    $newline = "\r\n";
    $newtime = $newtime.$newline;
    fwrite($highscorefile,$newtime);
    fclose($highscorefile);

} else {
    $highscorefile = fopen("highscores.txt","r") or die("unable to open file");
    echo fread($highscorefile, filesize("highscores.txt"));
    fclose($highscorefile);
}
?>
