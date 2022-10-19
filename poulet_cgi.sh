#!/bin/bash

if [ "$#" != "1" ]; then
	echo "one argument needed" >&2
	exit 1
fi

if [[ ! -f "$1" ]]; then
	echo "file doesn't exist" >&2
	exit 1
fi

cat << EOF
<!DOCTYPE html>
<html>
	<head>
		<title>PAULO LE POULET</title>
	</head>
	<body>
	$(awk '{ print "\t<h2>" $0 " Paulo Le Poulet" "</h2>" }' "$1")
	</body>
</html>
EOF
