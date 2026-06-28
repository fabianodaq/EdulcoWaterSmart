#ifndef APP_EMAILER_PRIVATE_H
#define APP_EMAILER_PRIVATE_H

#define SENDER_LIMIT_EMAILS_RESET_TIME 86.400 // number of mails can be sent in a day
#define SENDER_LIMIT_EMAILS_DEFAULT 5 // number of mails can be sent in a day
#define SENDER_WATCHDOG_RESET_TIME    86400UL    // 1 giorno = 86400 s

#define SMTP_HOST                   "smtp.gmail.com"
#define SMTP_PORT                   465
#define EMAIL_FROM_NAME             "EdulcoWater System Mailer"

#define EMAILER_INTERVAL 1000

#endif // APP_EMAILER_H

