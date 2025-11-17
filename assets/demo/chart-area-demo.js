// Set new default font family and font color to mimic Bootstrap's default styling
Chart.defaults.global.defaultFontFamily = '-apple-system,system-ui,BlinkMacSystemFont,"Segoe UI",Roboto,"Helvetica Neue",Arial,sans-serif';
Chart.defaults.global.defaultFontColor = '#292b2c';

let myAreaChart;
let myAreaChart2;
let myAreaChart3;
let myPhChart;
let myPhChart2;

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

  // Inicializar segunda gráfica de pH (incubadora 3)
  var ctxPh2 = document.getElementById("myPhChart3");
  if (ctxPh2) {
    myPhChart2 = new Chart(ctxPh2, {
      type: 'line',
      data: {
        labels: [],
        datasets: [{
          label: "pH (Incubadora 3)",
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

  // Inicializar controles de filtro si existen
  initFilterControls();

});

// --- Configuración de Firebase y variables adicionales ---
const firebaseBaseURL = 'https://ranitas-test-default-rtdb.firebaseio.com'; // Ajusta si es necesario

// --- Funciones para datos ---
async function fetchIncubatorData(incubadora) {
  try {
    const url = `${firebaseBaseURL}/Incu${incubadora}.json`;
    const resp = await fetch(url);
    if (!resp.ok) throw new Error('Error en fetch: ' + resp.status);
    const json = await resp.json();
    if (!json) return [];
    // Firebase RTDB devuelve objeto con llaves; convertir a array
    const arr = Object.keys(json).map(k => {
      const item = json[k];
      // Normalizar campos: timestamp, temp, hum, ph
      return {
        timestamp: item.timestamp || item.time || item.fecha || item.date || null,
        temp: item.temp !== undefined ? Number(item.temp) : (item.temperature !== undefined ? Number(item.temperature) : null),
        hum: item.hum !== undefined ? Number(item.hum) : (item.humidity !== undefined ? Number(item.humidity) : null),
        ph: item.ph !== undefined ? Number(item.ph) : null
      };
    });
    // Ordenar por timestamp ascendente (si existe)
    arr.sort((a,b) => {
      const ta = a.timestamp ? Date.parse(a.timestamp) : 0;
      const tb = b.timestamp ? Date.parse(b.timestamp) : 0;
      return ta - tb;
    });
    return arr;
  } catch (err) {
    console.error('fetchIncubatorData error', err);
    return [];
  }
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

// --- Actualización de gráficas y tarjetas ---
function updateChartsFromData(incubadora, data) {
  const timestamps = data.map(d => d.timestamp);
  const temps = data.map(d => d.temp);
  const hums = data.map(d => d.hum);
  const phs = data.map(d => d.ph);

  if (incubadora == 1 && myAreaChart) updateAreaChart(timestamps, temps, hums);
  if (incubadora == 2 && myAreaChart2) updateAreaChart2(timestamps, temps, hums);
  if (incubadora == 3 && myAreaChart3) updateAreaChart3(timestamps, temps, hums);

  // Actualizar gráfico de pH general (si corresponde)
  if (incubadora != 3 && myPhChart) updatePhChart(timestamps, phs);
  if (incubadora == 3 && myPhChart2) {
    myPhChart2.data.labels = timestamps;
    myPhChart2.data.datasets[0].data = phs;
    myPhChart2.update();
  }

  // Actualizar info cards para la incubadora (si existen)
  if (data.length > 0) {
    const last = data[data.length - 1];
    updateInfoCards(incubadora, last);
  }
}

function updateInfoCards(incubadora, latest) {
  // Espera que el HTML tenga elementos con ids: tempCard{n}, humCard{n}, phCard{n}
  const tempEl = document.getElementById(`tempCard${incubadora}`);
  const humEl = document.getElementById(`humCard${incubadora}`);
  const phEl = document.getElementById(`phCard${incubadora}`);
  if (tempEl && latest.temp !== null && latest.temp !== undefined) tempEl.innerText = latest.temp.toFixed(2) + ' °C';
  if (humEl && latest.hum !== null && latest.hum !== undefined) humEl.innerText = latest.hum.toFixed(2) + ' %';
  if (phEl && latest.ph !== null && latest.ph !== undefined) phEl.innerText = latest.ph.toFixed(2);
}

// --- Controles de filtro (selector incubadora y rango de tiempo) ---
function initFilterControls() {
  const incubatorSelect = document.getElementById('incubatorSelect'); // select con opciones 1,2,3
  const startInput = document.getElementById('startTime'); // input type=datetime-local
  const endInput = document.getElementById('endTime');     // input type=datetime-local
  const applyBtn = document.getElementById('applyFilter');

  if (!applyBtn || !incubatorSelect) return; // No hay controles, salir

  applyBtn.addEventListener('click', async () => {
    const incubadora = Number(incubatorSelect.value) || 1;
    const startVal = startInput ? startInput.value : '';
    const endVal = endInput ? endInput.value : '';
    // Convertir datetime-local (local) a ISO string compatible con Date parsing
    const startISO = startVal ? new Date(startVal).toISOString() : '';
    const endISO = endVal ? new Date(endVal).toISOString() : '';

    // Cargar datos y filtrar
    const all = await fetchIncubatorData(incubadora);
    const filtered = filterByRange(all, startISO, endISO);
    updateChartsFromData(incubadora, filtered);
  });

  // Cargar valores iniciales: incubadora 1 y pH de incubadora 3
  (async () => {
    const data1 = await fetchIncubatorData(1);
    updateChartsFromData(1, data1.slice(-50)); // última ventana

    const data2 = await fetchIncubatorData(2);
    updateChartsFromData(2, data2.slice(-50));

    const data3 = await fetchIncubatorData(3);
    updateChartsFromData(3, data3.slice(-50));
  })();
}
