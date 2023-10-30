FROM python:3.8

WORKDIR /app

RUN apt-get update && apt-get install -y xvfb

RUN echo '#!/bin/bash\n\
if [ "$SIM_MODE" = "true" ]; then\n\
  Xvfb :1 -screen 0 1024x768x16 &\n\
  DISPLAY=:1 python3 src/main.py -s\n\
else\n\
  python3 src/main.py\n\
fi' > /start.sh && chmod +x /start.sh

COPY . .

RUN pip3 install --no-cache-dir -r requirements.txt

CMD ["/start.sh"]