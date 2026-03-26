module.exports = {
  apps: [
    {
      name:                'mqtt-api',
      script:              'src/server.js',
      instances:           1,
      autorestart:         true,
      watch:               false,
      max_memory_restart:  '256M',
      env: {
        NODE_ENV: 'production',
      },
      error_file: 'logs/err.log',
      out_file:   'logs/out.log',
      time:       true,       // prefix log lines with timestamps
    },
  ],
};
