// Set new default font family and font color to mimic Bootstrap's default styling
Chart.defaults.global.defaultFontFamily = '-apple-system,system-ui,BlinkMacSystemFont,"Segoe UI",Roboto,"Helvetica Neue",Arial,sans-serif';
Chart.defaults.global.defaultFontColor = '#292b2c';

let myAreaChart;
let myAreaChart2;
let myAreaChart3;
let myPhChart;
let myPhChart3;

// Inicializar el gráfico cuando el DOM esté listo
document.addEventListener('DOMContentLoaded', function() {
  var ctx = document.getElementById("myAreaChart");
  if (ctx) {
    myAreaChart = new Chart(ctx, {
      type: 'line',
      data: {
        labels: [],
        datasets: [{
          label: "Temperatura",
          lineTension: 0.3,
          backgroundColor: "rgba(2,117,216,0.2)",
          borderColor: "rgba(2,117,216,1)",
          pointRadius: 5,
          pointBackgroundColor: "rgba(2,117,216,1)",
          pointBorderColor: "rgba(255,255,255,0.8)",
          pointHoverRadius: 5,
          pointHoverBackgroundColor: "rgba(2,117,216,1)",
          pointHitRadius: 50,
          pointBorderWidth: 2,
          data: []
        },
        {
          label: "Humedad",
          lineTension: 0.3,
          backgroundColor: "rgba(220,53,69,0.2)",
          borderColor: "rgba(220,53,69,1)",
          pointRadius: 5,
          pointBackgroundColor: "rgba(220,53,69,1)",
          pointBorderColor: "rgba(255,255,255,0.8)",
          pointHoverRadius: 5,
          pointHoverBackgroundColor: "rgba(220,53,69,1)",
          pointHitRadius: 50,
          pointBorderWidth: 2,
          data: []
        }]
      },
      options: {
        scales: {
          xAxes: [{
            time: {
              unit: 'date'
            },
            gridLines: {
              display: false
            },
            ticks: {
              maxTicksLimit: 7
            }
          }],
          yAxes: [{
            ticks: {
              min: 0,
              max: 100,
              maxTicksLimit: 5
            },
            gridLines: {
              color: "rgba(0, 0, 0, .125)",
            }
          }],
        },
        legend: {
          display: true
        }
      }
    });
  }

  // Incubadora 2
  var ctx2 = document.getElementById("myAreaChart2");
  if (ctx2) {
    myAreaChart2 = new Chart(ctx2, {
      type: 'line',
      data: {
        labels: [],
        datasets: [{
          label: "Temperatura",
          lineTension: 0.3,
          backgroundColor: "rgba(2,117,216,0.2)",
          borderColor: "rgba(2,117,216,1)",
          pointRadius: 5,
          pointBackgroundColor: "rgba(2,117,216,1)",
          pointBorderColor: "rgba(255,255,255,0.8)",
          pointHoverRadius: 5,
          pointHoverBackgroundColor: "rgba(2,117,216,1)",
          pointHitRadius: 50,
          pointBorderWidth: 2,
          data: []
        },
        {
          label: "Humedad",
          lineTension: 0.3,
          backgroundColor: "rgba(220,53,69,0.2)",
          borderColor: "rgba(220,53,69,1)",
          pointRadius: 5,
          pointBackgroundColor: "rgba(220,53,69,1)",
          pointBorderColor: "rgba(255,255,255,0.8)",
          pointHoverRadius: 5,
          pointHoverBackgroundColor: "rgba(220,53,69,1)",
          pointHitRadius: 50,
          pointBorderWidth: 2,
          data: []
        }]
      },
      options: {
        scales: {
          xAxes: [{
            time: {
              unit: 'date'
            },
            gridLines: {
              display: false
            },
            ticks: {
              maxTicksLimit: 7
            }
          }],
          yAxes: [{
            ticks: {
              min: 0,
              max: 100,
              maxTicksLimit: 5
            },
            gridLines: {
              color: "rgba(0, 0, 0, .125)",
            }
          }],
        },
        legend: {
          display: true
        }
      }
    });
  }

  // Incubadora 3
  var ctx3 = document.getElementById("myAreaChart3");
  if (ctx3) {
    myAreaChart3 = new Chart(ctx3, {
      type: 'line',
      data: {
        labels: [],
        datasets: [{
          label: "Temperatura",
          lineTension: 0.3,
          backgroundColor: "rgba(2,117,216,0.2)",
          borderColor: "rgba(2,117,216,1)",
          pointRadius: 5,
          pointBackgroundColor: "rgba(2,117,216,1)",
          pointBorderColor: "rgba(255,255,255,0.8)",
          pointHoverRadius: 5,
          pointHoverBackgroundColor: "rgba(2,117,216,1)",
          pointHitRadius: 50,
          pointBorderWidth: 2,
          data: []
        },
        {
          label: "Humedad",
          lineTension: 0.3,
          backgroundColor: "rgba(220,53,69,0.2)",
          borderColor: "rgba(220,53,69,1)",
          pointRadius: 5,
          pointBackgroundColor: "rgba(220,53,69,1)",
          pointBorderColor: "rgba(255,255,255,0.8)",
          pointHoverRadius: 5,
          pointHoverBackgroundColor: "rgba(220,53,69,1)",
          pointHitRadius: 50,
          pointBorderWidth: 2,
          data: []
        }]
      },
      options: {
        scales: {
          xAxes: [{
            time: {
              unit: 'date'
            },
            gridLines: {
              display: false
            },
            ticks: {
              maxTicksLimit: 7
            }
          }],
          yAxes: [{
            ticks: {
              min: 0,
              max: 100,
              maxTicksLimit: 5
            },
            gridLines: {
              color: "rgba(0, 0, 0, .125)",
            }
          }],
        },
        legend: {
          display: true
        }
      }
    });
  }

  // Gráfica de pH
  var ctxPh = document.getElementById("myPhChart");
  if (ctxPh) {
    myPhChart = new Chart(ctxPh, {
      type: 'line',
      data: {
        labels: [],
        datasets: [{
          label: "pH",
          lineTension: 0.3,
          backgroundColor: "rgba(40,167,69,0.2)",
          borderColor: "rgba(40,167,69,1)",
          pointRadius: 5,
          pointBackgroundColor: "rgba(40,167,69,1)",
          pointBorderColor: "rgba(255,255,255,0.8)",
          pointHoverRadius: 5,
          pointHoverBackgroundColor: "rgba(40,167,69,1)",
          pointHitRadius: 50,
          pointBorderWidth: 2,
          data: []
        }]
      },
      options: {
        scales: {
          xAxes: [{
            time: {
              unit: 'date'
            },
            gridLines: {
              display: false
            },
            ticks: {
              maxTicksLimit: 7
            }
          }],
          yAxes: [{
            ticks: {
              min: 0,
              max: 14,
              maxTicksLimit: 8
            },
            gridLines: {
              color: "rgba(0, 0, 0, .125)",
            }
          }],
        },
        legend: {
          display: true
        }
      }
    });
  }

  // Gráfica de pH incubadora 3
  var ctxPh3 = document.getElementById("myPhChart3");
  if (ctxPh3) {
    myPhChart3 = new Chart(ctxPh3, {
      type: 'line',
      data: {
        labels: [],
        datasets: [{
          label: "pH",
          lineTension: 0.3,
          backgroundColor: "rgba(255,193,7,0.2)",
          borderColor: "rgba(255,193,7,1)",
          pointRadius: 5,
          pointBackgroundColor: "rgba(255,193,7,1)",
          pointBorderColor: "rgba(255,255,255,0.8)",
          pointHoverRadius: 5,
          pointHoverBackgroundColor: "rgba(255,193,7,1)",
          pointHitRadius: 50,
          pointBorderWidth: 2,
          data: []
        }]
      },
      options: {
        scales: {
          xAxes: [{
            time: { unit: 'date' },
            gridLines: { display: false },
            ticks: { maxTicksLimit: 7 }
          }],
          yAxes: [{
            ticks: { min: 0, max: 14, maxTicksLimit: 8 },
            gridLines: { color: "rgba(0, 0, 0, .125)" }
          }]
        },
        legend: { display: true }
      }
    });
  }

  // Filtros funcionales
  const applyBtn = document.getElementById('applyFilter');
  if (applyBtn) {
    applyBtn.addEventListener('click', async function() {
      const incubadora = document.getElementById('incubatorSelect').value;
      const startVal = document.getElementById('startTime').value;
      const endVal = document.getElementById('endTime').value;
      const startISO = startVal ? new Date(startVal).toISOString() : null;
      const endISO = endVal ? new Date(endVal).toISOString() : null;
      await updateAllChartsFiltered(incubadora, startISO, endISO);
    });
  }
});

// Función para actualizar el gráfico con nuevos datos
function updateAreaChart(timestamps, temperatures, humidities) {
  if (myAreaChart) {
    myAreaChart.data.labels = timestamps;
    myAreaChart.data.datasets[0].data = temperatures;
    myAreaChart.data.datasets[1].data = humidities;
    myAreaChart.update();
  }
}

// Función para actualizar el gráfico de Incubadora 2
function updateAreaChart2(timestamps, temperatures, humidities) {
  if (myAreaChart2) {
    myAreaChart2.data.labels = timestamps;
    myAreaChart2.data.datasets[0].data = temperatures;
    myAreaChart2.data.datasets[1].data = humidities;
    myAreaChart2.update();
  }
}

// Función para actualizar el gráfico de Incubadora 3
function updateAreaChart3(timestamps, temperatures, humidities) {
  if (myAreaChart3) {
    myAreaChart3.data.labels = timestamps;
    myAreaChart3.data.datasets[0].data = temperatures;
    myAreaChart3.data.datasets[1].data = humidities;
    myAreaChart3.update();
  }
}

// Función para actualizar el gráfico de pH
function updatePhChart(timestamps, phValues) {
  if (myPhChart) {
    myPhChart.data.labels = timestamps;
    myPhChart.data.datasets[0].data = phValues;
    myPhChart.update();
  }
}

// --- Funciones para cargar y filtrar datos desde Firebase RTDB ---
async function fetchIncubatorData(incubadora) {
  const url = `https://ranitas-test-default-rtdb.firebaseio.com/Incu${incubadora}.json`;
  const resp = await fetch(url);
  if (!resp.ok) return [];
  const json = await resp.json();
  if (!json) return [];
  const arr = Object.keys(json).map(k => {
    const item = json[k];
    return {
      timestamp: item.timestamp || item.time || item.fecha || item.date || null,
      temp: item.temp !== undefined ? Number(item.temp) : (item.PromT !== undefined ? Number(item.PromT) : null),
      hum: item.hum !== undefined ? Number(item.hum) : (item.PromH !== undefined ? Number(item.PromH) : null),
      ph: item.ph !== undefined ? Number(item.ph) : (item.pH !== undefined ? Number(item.pH) : null)
    };
  });
  arr.sort((a,b) => {
    const ta = a.timestamp ? Date.parse(a.timestamp) : 0;
    const tb = b.timestamp ? Date.parse(b.timestamp) : 0;
    return ta - tb;
  });
  return arr;
}

function filterByRange(data, startISO, endISO) {
  if (!startISO && !endISO) return data;
  const start = startISO ? new Date(startISO) : null;
  const end = endISO ? new Date(endISO) : null;
  return data.filter(d => {
    if (!d.timestamp) return false;
    const t = new Date(d.timestamp);
    if (isNaN(t)) return false;
    if (start && t < start) return false;
    if (end && t > end) return false;
    return true;
  });
}

// --- Actualización de gráficas filtradas ---
async function updateAllChartsFiltered(incubadora, startISO, endISO) {
  // Temperatura y humedad
  for (let i = 1; i <= 3; i++) {
    const data = await fetchIncubatorData(i);
    const filtered = filterByRange(data, startISO, endISO);
    const timestamps = filtered.map(d => d.timestamp);
    const temps = filtered.map(d => d.temp);
    const hums = filtered.map(d => d.hum);
    if (i === 1) updateAreaChart(timestamps, temps, hums);
    if (i === 2) updateAreaChart2(timestamps, temps, hums);
    if (i === 3) updateAreaChart3(timestamps, temps, hums);
  }
  // pH incubadora 1
  if (myPhChart) {
    const data1 = await fetchIncubatorData(1);
    const filtered1 = filterByRange(data1, startISO, endISO);
    const timestamps1 = filtered1.map(d => d.timestamp);
    const phs1 = filtered1.map(d => d.ph);
    updatePhChart(timestamps1, phs1);
  }
  // pH incubadora 3
  if (myPhChart3) {
    const data3 = await fetchIncubatorData(3);
    const filtered3 = filterByRange(data3, startISO, endISO);
    const timestamps3 = filtered3.map(d => d.timestamp);
    const phs3 = filtered3.map(d => d.ph);
    myPhChart3.data.labels = timestamps3;
    myPhChart3.data.datasets[0].data = phs3;
    myPhChart3.update();
  }
}
