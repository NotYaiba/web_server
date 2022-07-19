<?php
$color = "red";
echo "My car is " . $color . "<br>";
echo "My house is " . $COLOR . "<br>";
echo "My boat is " . $coLOR . "<br>";

// phpinfo();

// echo getenv('REQUEST_METHOD');
// echo getenv('SERVER_PROTOCOL');
// var_dump($_ENV);
while (list($var,$value) = each ($_ENV)) 
{
    echo "$var => $value <br />";
}
// phpinfo();

?>