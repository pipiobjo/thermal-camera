// default 404 response
// used for $upload.htm
static const char notFoundContent[] PROGMEM = R"==(
<html>
<head>
  <title>Ressource not found</title>
</head>
<body>
  <p>The ressource was not found.</p>
  <p><a href="/">Start again</a></p>
</body>
</html>
)==";