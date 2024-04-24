const express = require('express');
const mysql = require('mysql2/promise');

const app = express();
const port = 3000;

// Middleware untuk mengurai JSON dan URL-encoded bodies
app.use(express.json());
app.use(express.urlencoded({ extended: true }));

// Konfigurasi koneksi MySQL
const pool = mysql.createPool({
  host: 'localhost',
  user: 'root', // Ganti dengan username MySQL Anda
  password: 'root', // Ganti dengan password MySQL Anda
  database: 'esp32', // Ganti dengan nama database Anda
  waitForConnections: true,
  connectionLimit: 10,
  queueLimit: 0
});

// Endpoint untuk mendapatkan semua data dari tabel esp32_table_test
app.get('/esp32data', async (req, res) => {
  try {
    const connection = await pool.getConnection();
    const [rows] = await connection.query('SELECT * FROM esp32_table_test');
    connection.release();
    res.json(rows);
  } catch (error) {
    console.error('Error retrieving data:', error);
    res.status(500).json({ error: 'Internal Server Error' });
  }
});

// Endpoint untuk menambahkan data ke tabel esp32_table_test
app.post('/esp32data', async (req, res) => {
    const { id, temperature, humidity, status_read_sensor_dht11, LED_01, LED_02 } = req.body;
    try {
      const connection = await pool.getConnection();
  
      // Melakukan pembaruan data jika action adalah 'update'
      const [result] = await connection.query('UPDATE esp32_table_test SET temperature = ?, humidity = ?, status_read_sensor_dht11 = ?, LED_01 = ?, LED_02 = ? WHERE id = ?', [temperature, humidity, status_read_sensor_dht11, LED_01, LED_02, id]);
      connection.release();
      if (result.affectedRows === 0) {
        res.status(404).json({ error: 'Data not found' });
      } else {
        res.json({ message: 'Data updated successfully' });
      }
    } catch (error) {
      console.error('Error adding data:', error);
      res.status(500).json({ error: 'Internal Server Error' });
    }
  });
  
  

// Mulai server
app.listen(port, () => {
  console.log(`Server is running on port ${port}`);
});
