// Set new default font family and font color to mimic Bootstrap's default styling
Chart.defaults.global.defaultFontFamily = '-apple-system,system-ui,BlinkMacSystemFont,"Segoe UI",Roboto,"Helvetica Neue",Arial,sans-serif';
Chart.defaults.global.defaultFontColor = '#292b2c';

let myAreaChart;

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
