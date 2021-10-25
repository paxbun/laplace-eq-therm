// temp is in [18, 30]
function temperatureToColor(temp, min, max) {
  min = min - 1.0;
  max = max + 1.0;
  temp = Math.min(temp, max);
  temp = Math.max(temp, min);
  temp = (temp - min) / (max - min);

  const hue = (1 - temp) * 100;
  const sat = 100;
  const brightness = temp * 50;

  return `hsl(${hue}, ${sat}%, ${brightness}%)`;
}

function createTable(tableElem, title, width, height, callback) {
  const captionElem = document.createElement("caption");
  captionElem.appendChild(document.createTextNode(title));
  tableElem.appendChild(captionElem);

  const bodyElem = document.createElement("tbody");
  tableElem.appendChild(bodyElem);

  const table = [];
  for (let i = 0; i < height; ++i) {
    const rowElem = document.createElement("tr");
    const row = [];
    for (let j = 0; j < width; ++j) {
      const cellElem = document.createElement("td");
      cellElem.style.color = "white";
      cellElem.style.width = "60px";
      cellElem.style.height = "60px";
      cellElem.style.backgroundColor = "black";
      cellElem.appendChild(document.createTextNode((0).toFixed(2)));

      if (callback) callback(i, j, cellElem);
      row.push(cellElem);
      rowElem.appendChild(cellElem);
    }
    table.push(row);
    bodyElem.appendChild(rowElem);
  }

  return table;
}

function removeErrorFromTable(table) {
  const divElem =
    table[0][0].parentElement.parentElement.parentElement.parentElement;
  const errorElem = divElem.querySelector(".error");
  if (errorElem !== null) {
    divElem.removeChild(errorElem);
  }
}

function applyErrorToTable(table, errorCode, errorMessage) {
  const divElem =
    table[0][0].parentElement.parentElement.parentElement.parentElement;
  const errorElem = document.createElement("div");
  errorElem.className = "error";
  errorElem.appendChild(
    document.createTextNode(`${errorMessage} (${errorCode})`)
  );
  divElem.appendChild(errorElem);
}

function applyResultsToTable(table, width, height, temp, type) {
  let min = Math.min(...temp.map((arr) => Math.min(...arr)));
  let max = Math.max(...temp.map((arr) => Math.max(...arr)));
  for (let i = 0; i < height; ++i) {
    for (let j = 0; j < width; ++j) {
      const cellElem = table[i][j];
      const cellValue = parseFloat(temp[i][j]);
      const cellColor = temperatureToColor(cellValue, min, max);

      while (cellElem.firstChild !== null)
        cellElem.removeChild(cellElem.firstChild);

      switch (type[i][j]) {
        case "Boundary":
          cellElem.style.opacity = "1";
          cellElem.style.fontWeight = "bold";
          cellElem.style.fontSize = "20px";
          cellElem.appendChild(document.createTextNode(cellValue.toFixed(2)));
          cellElem.style.backgroundColor = cellColor;
          break;
        case "GroundTruth":
          cellElem.style.opacity = "1";
          cellElem.style.fontWeight = "normal";
          cellElem.style.fontSize = "16px";
          cellElem.appendChild(document.createTextNode(cellValue.toFixed(2)));
          cellElem.style.backgroundColor = cellColor;
          break;
        case "OutOfRange":
          cellElem.style.opacity = "0";
          break;
      }
    }
  }
}

async function getResults() {
  let rtn = await fetch("/state");
  return await rtn.json();
}

function sleep(ms) {
  return new Promise((resolve) => setTimeout(resolve, ms));
}

let type;
let temp;
const nextType = {
  Boundary: "OutOfRange",
  GroundTruth: "Boundary",
  OutOfRange: "GroundTruth",
};

async function init() {
  const initialResults = await (async () => {
    try {
      return await getResults();
    } catch (e) {
      alert("Initialization failed. Could not connect to the server.");
    }
  })();
  const width = initialResults.width;
  const height = initialResults.height;

  const inputTable = createTable(
    document.getElementById("input"),
    "Input",
    width,
    height,
    (i, j, cellElem) => {
      cellElem.addEventListener("click", async () => {
        const next = nextType[type[i][j]];
        await fetch("/state", {
          method: "POST",
          body: JSON.stringify({
            x: j,
            y: i,
            temp:
              type[i][j] === "GroundTruth"
                ? parseFloat(
                    prompt(`New temperature for ${next} point (${i}, ${j}):`)
                  )
                : temp[i][j],
            type: next,
          }),
        });
      });
    }
  );
  const outputTables = {};
  for (const result of initialResults.results) {
    const divElem = document.createElement("div");
    document.body.appendChild(divElem);
    const tableElem = document.createElement("table");
    divElem.appendChild(tableElem);

    const table = createTable(tableElem, result.name, width, height);
    outputTables[result.name] = table;
  }

  let results = initialResults;
  type = results.info.type;
  temp = results.info.temp;
  while (true) {
    applyResultsToTable(
      inputTable,
      width,
      height,
      results.info.temp,
      results.info.type
    );
    for (const result of results.results) {
      const table = outputTables[result.name];
      if (result.errorCode === 0) {
        removeErrorFromTable(table);
        applyResultsToTable(
          table,
          width,
          height,
          result.temp,
          results.info.type
        );
      } else {
        removeErrorFromTable(table);
        applyErrorToTable(table, result.errorCode, result.errorMessage);
      }
    }
    await sleep(1000);
    try {
      results = await getResults();
      type = results.info.type;
      temp = results.info.temp;
    } catch (e) {
      alert("Could not connect to the server.");
    }
  }
}

window.addEventListener("load", init);
