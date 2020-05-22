const http = require("http");
const url = require("url");
const Weather = require("./api");
const { password } = require("./config");
const PORT = process.env.PORT || 5000;

const server = http.createServer((req, res) => {
  // check for the correct password in the query params
  const query = url.parse(req.url,true).query;
  if (query.password !== password) {
    res.writeHead(401);
    res.write("need password");
    res.end();
    return;
  }
  // password okay. request data from the API
  return Weather()
    .then((data) => {
      const stringified = JSON.stringify(data) + "\n";
      res.writeHead(200, {
        "Content-Type": "application/json",
        "Content-Length": `${stringified.length}`
      });
      res.write(stringified);
      res.end();
    })
    .catch((error) => {
      res.writeHead(501);
      res.write(error.message);
      res.end();
    });
});
server.listen(PORT);
