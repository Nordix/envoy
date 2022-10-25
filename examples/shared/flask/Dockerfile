FROM python:3.11.0-slim-bullseye@sha256:40947336b1099d5feb73222029e57172144ba05cd33276d8848f8a8e1fbf842e

ADD requirements.txt /tmp/flask-requirements.txt
RUN pip3 install -qr /tmp/flask-requirements.txt
RUN mkdir /code

ENTRYPOINT ["python3", "/code/service.py"]
