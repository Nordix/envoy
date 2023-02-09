FROM python:3.11.2-slim-bullseye@sha256:a369defaecd2d298d551828fa7af7de776bd5575f28b0b804f2ae993d32f5607

ADD requirements.txt /tmp/flask-requirements.txt
RUN pip3 install -qr /tmp/flask-requirements.txt
RUN mkdir /code

ENTRYPOINT ["python3", "/code/service.py"]
