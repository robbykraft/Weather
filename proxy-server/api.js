const https = require("https");
const { apiKey, latlon } = require("./config");

// darksky returns 48 entries in their hourly array
// EXPIRATION_TIME must be smaller than one day (82800)
const EXPIRATION_TIME = 3600 * 5; // five hours
const URL = `https://api.darksky.net/forecast/${apiKey}/${latlon}?exclude=currently,minutely,daily,alerts,flags`;
// this is being sent to an Arduino. let's shrink the API response.
const keepKeys = ["temperature", "precipProbability"];
// "cloudCover", "precipType", "summary",

let lastCall = 0; // unix time, seconds not ms
// the cache will keep >24 entries (48 from the API). shrink to 24 upon request
const cache = [];

// seconds not ms
const NowTime = () => Math.floor((new Date).getTime() / 1000)

// shrink each entry in the API response
const simplified = function (element) {
  const simp = {};
  keepKeys.forEach((key) => { simp[key] = element[key]; })
  return simp;
};

const sliceCacheToNow = () => {
  const nowtime = NowTime();
  return cache.filter(el => nowtime < el.time)
    .slice(0, 24)
    .map(el => simplified(el));
};

const getData = function () {
  return new Promise((resolve, reject) => {
    // have we called this API too recently?
    const nowtime = NowTime();
    const needsNewData = (nowtime - lastCall) > EXPIRATION_TIME;
    lastCall = nowtime;
    // if so, use cache
    if (!needsNewData){
      const contemporaneous = sliceCacheToNow();
      if (contemporaneous.length === 24) {
        return resolve(contemporaneous);
      }
      // if for any reason we don't have 24 entries, re-call API
    }

    https.get(URL, (resp) => {
      let data = "";
      resp.on("data", (chunk) => { data += chunk; });
      resp.on("end", () => {
        while (cache.length > 0) { cache.pop(); }
        JSON.parse(data).hourly.data.forEach(el => cache.push(el));
        return resolve(sliceCacheToNow());
      });
    }).on("error", (err) => {
      reject(err.message);
    });    
  });
};

module.exports = getData;
