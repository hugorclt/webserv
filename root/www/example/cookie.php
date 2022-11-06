<?php
// génère le cookie
$contenu = 'Julien'; // le contenu de votre cookie
setcookie("nom_cookie", $contenu, time()+36000);
?>
<html>
<body>
<p>
Un cookie a été généré sur cette page.
</p>
</body>
</html>
