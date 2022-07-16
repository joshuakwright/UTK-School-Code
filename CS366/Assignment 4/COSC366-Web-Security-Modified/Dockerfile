FROM node:13


# Create app directory
ADD code/ /home/cosc366/web-security/
WORKDIR /home/cosc366/web-security/

# Install app dependencies
# A wildcard is used to ensure both package.json AND package-lock.json are copied
# where available (npm@5+)

RUN npm install
RUN apt-get -y update
RUN apt-get install -y sqlite3

# Bundle app source
COPY . .

EXPOSE 3000
CMD [ "npm", "start" ]
