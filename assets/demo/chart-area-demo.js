// Set new default font family and font color to mimic Bootstrap's default styling
Chart.defaults.global.defaultFontFamily = '-apple-system,system-ui,BlinkMacSystemFont,"Segoe UI",Roboto,"Helvetica Neue",Arial,sans-serif';
Chart.defaults.global.defaultFontColor = '#292b2c';

let myAreaChart;
let myAreaChart2;
let myAreaChart3;
let myPhChart;
let myPhChart3;
let filteredTempChart, filteredHumChart, filteredPhChart;

// Inicializar el gráfico cuando el DOM esté listo
document.addEventListener('DOMContentLoaded', async function () {
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

  // Inicializar nuevas gráficas filtradas al cargar DOM
  var ctxFilteredTemp = document.getElementById("filteredTempChart");
  var ctxFilteredHum = document.getElementById("filteredHumChart");
  var ctxFilteredPh = document.getElementById("filteredPhChart");
  if (ctxFilteredTemp) {
    filteredTempChart = new Chart(ctxFilteredTemp, {
      type: 'line',
      data: { labels: [], datasets: [{ label: 'Temperatura', borderColor: '#007bff', data: [] }] },
      options: { legend: { display: true } }
    });
  }
  if (ctxFilteredHum) {
    filteredHumChart = new Chart(ctxFilteredHum, {
      type: 'line',
      data: { labels: [], datasets: [{ label: 'Humedad', borderColor: '#28a745', data: [] }] },
      options: { legend: { display: true } }
    });
  }
  if (ctxFilteredPh) {
    filteredPhChart = new Chart(ctxFilteredPh, {
      type: 'line',
      data: { labels: [], datasets: [{ label: 'pH', borderColor: '#ffc107', data: [] }] },
      options: { legend: { display: true }, scales: { yAxes: [{ ticks: { min: 0, max: 14 } }] } }
    });
  }

  // Filtro funcional para nuevas gráficas
  const applyBtn = document.getElementById('applyFilter');
  if (applyBtn) {
    applyBtn.addEventListener('click', async function () {
      const startVal = document.getElementById('startTime').value;
      const endVal = document.getElementById('endTime').value;
      const startISO = startVal ? new Date(startVal).toISOString() : null;
      const endISO = endVal ? new Date(endVal).toISOString() : null;
      const filtered = await fetchHistorialByRange(startISO, endISO);
      const timestamps = filtered.map(d => d.timestamp);
      const temps = filtered.map(d => d.temp);
      const hums = filtered.map(d => d.hum);
      const phs = filtered.map(d => d.ph);
      if (filteredTempChart) {
        filteredTempChart.data.labels = timestamps;
        filteredTempChart.data.datasets[0].data = temps;
        filteredTempChart.options.scales = { yAxes: [{ ticks: { min: 0, max: 40 } }] };
        filteredTempChart.update();
      }
      if (filteredHumChart) {
        filteredHumChart.data.labels = timestamps;
        filteredHumChart.data.datasets[0].data = hums;
        filteredHumChart.options.scales = { yAxes: [{ ticks: { min: 0, max: 100 } }] };
        filteredHumChart.update();
      }
      if (filteredPhChart) {
        filteredPhChart.data.labels = timestamps;
        filteredPhChart.data.datasets[0].data = phs;
        filteredPhChart.options.scales = { yAxes: [{ ticks: { min: 0, max: 14 } }] };
        filteredPhChart.update();
      }
    });
  }

  // Mostrar pH de incubadora 3 en tarjetas
  const data3 = await fetchIncubatorData(3);
  if (data3.length > 0) {
    const last = data3[data3.length - 1];
    const ph3 = last.ph !== undefined && last.ph !== null ? last.ph : '--';
    const ph3El = document.getElementById('pH_3');
    if (ph3El) ph3El.innerText = ph3;
    const phCardBody3 = document.getElementById('phCardBody3');
    if (phCardBody3) phCardBody3.innerText = (ph3 !== '--') ? `pH actual: ${ph3}` : 'Sin datos de pH';
  }

  // --- Funciones para actualizar gráficas principales desde el historial global ---
  async function updateAreaChart2FromHistorial() {
    const url = 'https://ranitas-test-default-rtdb.firebaseio.com/historial.json';
    const resp = await fetch(url);
    if (!resp.ok) return;
    const json = await resp.json();
    if (!json) return;
    const datos = Object.values(json).filter(r => r.origen === 'Incu2' && r.PromT > 0 && r.PromH > 0 && r.timestamp);
    const timestamps = datos.map(r => new Date(r.timestamp).toLocaleTimeString());
    const temps = datos.map(r => r.PromT);
    const hums = datos.map(r => r.PromH);
    window.updateAreaChart2(timestamps, temps, hums);
  }
  async function updateAreaChart3FromHistorial() {
    const url = 'https://ranitas-test-default-rtdb.firebaseio.com/historial.json';
    const resp = await fetch(url);
    if (!resp.ok) return;
    const json = await resp.json();
    if (!json) return;
    const datos = Object.values(json).filter(r => r.origen === 'Incu3' && r.PromT > 0 && r.PromH > 0 && r.timestamp);
    const timestamps = datos.map(r => new Date(r.timestamp).toLocaleTimeString());
    const temps = datos.map(r => r.PromT);
    const hums = datos.map(r => r.PromH);
    window.updateAreaChart3(timestamps, temps, hums);
  }
  // Función para actualizar gráfica de pH de incubadora 3 (myPhChart3)
  // Filtro: últimas 24 horas y pH > 7.3
  async function updatePhChart3FromHistorial() {
    const url = 'https://ranitas-test-default-rtdb.firebaseio.com/historial.json';
    const resp = await fetch(url);
    if (!resp.ok) return;
    const json = await resp.json();
    if (!json) return;
    const now = new Date();
    const oneDayMs = 24 * 60 * 60 * 1000;
    // Filtrar: últimas 24 horas y pH > 7.3
    const datos = Object.values(json).filter(r => {
      if (!r.timestamp || !r.pH) return false;
      const readingDate = new Date(r.timestamp);
      const phValue = parseFloat(r.pH);
      return !isNaN(readingDate) &&
        (now - readingDate) <= oneDayMs &&
        !isNaN(phValue) &&
        phValue > 7.3;
    });
    // Ordenar por timestamp
    datos.sort((a, b) => new Date(a.timestamp) - new Date(b.timestamp));
    const timestamps = datos.map(r => new Date(r.timestamp).toLocaleTimeString());
    const phs = datos.map(r => parseFloat(r.pH));
    window.updatePhChart3(timestamps, phs);
  }

  // Función para actualizar gráfica de pH de incubadora 1 y 2 (myPhChart)
  // Filtro: últimas 24 horas y pH < 7.3
  async function updatePhChartFromHistorial() {
    const url = 'https://ranitas-test-default-rtdb.firebaseio.com/historial.json';
    const resp = await fetch(url);
    if (!resp.ok) return;
    const json = await resp.json();
    if (!json) return;
    const now = new Date();
    const oneDayMs = 24 * 60 * 60 * 1000;
    // Filtrar: últimas 24 horas, pH < 7.3 y pH > 0
    const datos = Object.values(json).filter(r => {
      if (!r.timestamp || !r.pH) return false;
      const readingDate = new Date(r.timestamp);
      const phValue = parseFloat(r.pH);
      return !isNaN(readingDate) &&
        (now - readingDate) <= oneDayMs &&
        !isNaN(phValue) &&
        phValue < 7.3 &&
        phValue > 0;
    });
    // Ordenar por timestamp
    datos.sort((a, b) => new Date(a.timestamp) - new Date(b.timestamp));
    const timestamps = datos.map(r => new Date(r.timestamp).toLocaleTimeString());
    const phs = datos.map(r => parseFloat(r.pH));
    window.updatePhChart(timestamps, phs);
  }

  // Llamar estas funciones al cargar la página
  if (document.readyState === 'complete' || document.readyState === 'interactive') {
    updateAreaChart2FromHistorial();
    updateAreaChart3FromHistorial();
    updatePhChart3FromHistorial();
    updatePhChartFromHistorial(); // <-- Gráfica de pH incubadora 1 y 2
  } else {
    document.addEventListener('DOMContentLoaded', () => {
      updateAreaChart2FromHistorial();
      updateAreaChart3FromHistorial();
      updatePhChart3FromHistorial();
      updatePhChartFromHistorial(); // <-- Gráfica de pH incubadora 1 y 2
    });
  }

  // Actualizar gráficas de pH cada 5 minutos
  setInterval(() => {
    updatePhChartFromHistorial();
    updatePhChart3FromHistorial();
  }, 5 * 60 * 1000);
});

// Función para actualizar el gráfico con nuevos datos
window.updateAreaChart = function (timestamps, temperatures, humidities) {
  if (myAreaChart) {
    myAreaChart.data.labels = timestamps;
    myAreaChart.data.datasets[0].data = temperatures;
    myAreaChart.data.datasets[1].data = humidities;
    myAreaChart.update();
  }
}
window.updateAreaChart2 = function (timestamps, temperatures, humidities) {
  if (myAreaChart2) {
    myAreaChart2.data.labels = timestamps;
    myAreaChart2.data.datasets[0].data = temperatures;
    myAreaChart2.data.datasets[1].data = humidities;
    myAreaChart2.update();
  }
}
window.updateAreaChart3 = function (timestamps, temperatures, humidities) {
  if (myAreaChart3) {
    myAreaChart3.data.labels = timestamps;
    myAreaChart3.data.datasets[0].data = temperatures;
    myAreaChart3.data.datasets[1].data = humidities;
    myAreaChart3.update();
  }
}
window.updatePhChart = function (timestamps, phValues) {
  if (myPhChart) {
    myPhChart.data.labels = timestamps;
    myPhChart.data.datasets[0].data = phValues;
    myPhChart.update();
  }
}
window.updatePhChart3 = function (timestamps, phValues) {
  if (myPhChart3) {
    myPhChart3.data.labels = timestamps;
    myPhChart3.data.datasets[0].data = phValues;
    myPhChart3.update();
  }
}

// --- Función para obtener datos de Firebase para incubadora N ---
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
  arr.sort((a, b) => {
    const ta = a.timestamp ? Date.parse(a.timestamp) : 0;
    const tb = b.timestamp ? Date.parse(b.timestamp) : 0;
    return ta - tb;
  });
  return arr;
}

// --- Nueva función para obtener y filtrar datos del historial global por rango de fechas ---
async function fetchHistorialByRange(startISO, endISO) {
  const url = 'https://ranitas-test-default-rtdb.firebaseio.com/historial.json';
  const resp = await fetch(url);
  if (!resp.ok) return [];
  const json = await resp.json();
  if (!json) return [];
  const arr = Object.values(json).map(item => ({
    timestamp: item.timestamp || item.time || item.fecha || item.date || null,
    temp: item.PromT !== undefined ? Number(item.PromT) : null,
    hum: item.PromH !== undefined ? Number(item.PromH) : null,
    ph: item.pH !== undefined ? Number(item.pH) : null
  }));
  arr.sort((a, b) => {
    const ta = a.timestamp ? Date.parse(a.timestamp) : 0;
    const tb = b.timestamp ? Date.parse(b.timestamp) : 0;
    return ta - tb;
  });
  // Filtrar por rango de fechas
  const start = startISO ? new Date(startISO) : null;
  const end = endISO ? new Date(endISO) : null;
  return arr.filter(d => {
    if (!d.timestamp) return false;
    const t = new Date(d.timestamp);
    if (isNaN(t)) return false;
    if (start && t < start) return false;
    if (end && t > end) return false;
    return true;
  });
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
// (NO USAR updateAllChartsFiltered en el filtro, solo para referencia interna si se requiere)
