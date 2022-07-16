import express from 'express';
import sqlite from 'sqlite';

import { asyncMiddleware } from './utils/asyncMiddleware';
import sleep from './utils/sleep';

const router = express.Router();
const dbPromise = sqlite.open('./db/database.db')

function render(req, res, next, page, title, errorMsg = false, result = null) {
  res.render(
    'layout/template', {
      page,
      title,
      loggedIn: req.session.loggedIn,
      account: req.session.account,
      errorMsg,
      result,
    }
  );
}


router.get('/', (req, res, next) => {
  render(req, res, next, 'index', 'VolCoin Home');
});


router.post('/set_profile', asyncMiddleware(async (req, res, next) => {
  req.session.account.profile = req.body.new_profile;
  console.log(req.body.new_profile);
  const db = await dbPromise;
  // const profile = req.body.new_profile.replace(/"/g, "'");
  // const query = `UPDATE Users SET profile = "${profile}" WHERE username = "${req.session.account.username}";`;
  const query = `UPDATE Users SET profile = "${req.body.new_profile}" WHERE username = "${req.session.account.username}";`;
  console.log(query)
  const result = await db.run(query);
  render(req, res, next, 'index', 'VolCoin Home');

}));


router.get('/login', (req, res, next) => {
  render(req, res, next, 'login/form', 'Login');
});


router.get('/get_login', asyncMiddleware(async (req, res, next) => {
  const db = await dbPromise;
  let username = req.query.username;
  let password = req.query.password;
  for (let i = 0; i < username.length; i++) {
    if(!((username.codePointAt(i) >= 48 && username.codePointAt(i) <= 57) || (username.codePointAt(i) >= 65 
    && username.codePointAt(i) <= 90) || (username.codePointAt(i) >= 97 && username.codePointAt(i) <= 122))) {
      username = username.substring(0,i) + "\\" + username.substring(i, username.length);
      i += 2;
    }
  }
  console.log(username);
  for (let i = 0; i < password.length; i++) {
    if(!((password.codePointAt(i) >= 48 && password.codePointAt(i) <= 57) || (password.codePointAt(i) >= 65 
    && password.codePointAt(i) <= 90) || (password.codePointAt(i) >= 97 && password.codePointAt(i) <= 122))) {
      password = password.substring(0,i) + "\\" + password.substring(i, password.length);
      i += 2;
    }
  }
  console.log(password);
  const query = `SELECT * FROM Users WHERE username == "${username}" AND password == "${password}";`;
  console.log(query)
  const result = await db.get(query);
  if(result) { // if this username actually exists
    await sleep(500);
    req.session.loggedIn = true;
    req.session.account = result;
    render(req, res, next, 'login/success', 'VolCoin Home');
    return;
  }
  render(req, res, next, 'login/form', 'Login', 'This username and password combination does not exist!');
}));


router.get('/logout', (req, res, next) => {
  req.session.loggedIn = false;
  req.session.account = {};
  render(req, res, next, 'index', 'VolCoin Home', 'Logged out successfully!');
});


router.get('/profile', asyncMiddleware(async (req, res, next) => {
  if(req.session.loggedIn == false) {
    render(req, res, next, 'login/form', 'Login', 'You must be logged in to use this feature!');
    return;
  };

  if(req.query.username != null) { // if visitor makes a search query
    const db = await dbPromise;
    const query = `SELECT * FROM Users WHERE username == "${req.query.username}";`;
    let result;
    try {
      result = await db.get(query);
    } catch(err) {
      result = false;
    }
    if(result) { // if user exists
      render(req, res, next, 'profile/view', 'View Profile', false, result);
    }
    else { // user does not exist
      render(req, res, next, 'profile/view', 'View Profile', `${req.query.username} does not exist!`, req.session.account);
    }
  } else { // visitor did not make query, show them their own profile
    render(req, res, next, 'profile/view', 'View Profile', false, req.session.account);
  }
}));


router.get('/transfer', (req, res, next) => {
  if(req.session.loggedIn == false) {
    render(req, res, next, 'login/form', 'Login', 'You must be logged in to use this feature!');
    return;
  };
  render(req, res, next, 'transfer/form', 'Transfer VolCoins', false, {receiver:null, amount:null});
});


router.post('/post_transfer', asyncMiddleware(async(req, res, next) => {
  if(req.session.loggedIn == false) {
    render(req, res, next, 'login/form', 'Login', 'You must be logged in to use this feature!');
    return;
  };

  if(req.body.destination_username === req.session.account.username) {
    render(req, res, next, 'transfer/form', 'Transfer VolCoins', 'You cannot send money to yourself!', {receiver:null, amount:null});
    return;
  }

  const db = await dbPromise;
  let query = `SELECT * FROM Users WHERE username == "${req.body.destination_username}";`;
  const receiver = await db.get(query);
  if(receiver) { // if user exists
    const amount = parseInt(req.body.quantity);
    if(Number.isNaN(amount) || amount > req.session.account.volCoins) {
      render(req, res, next, 'transfer/form', 'Transfer VolCoins', 'Invalid transfer amount!', {receiver:null, amount:null});
      return;
    }

    //0); req.session.account.volCoint = 1; req.session.account.username = "bob"; receiver.volCoins = 100; receiver.username = "alice";
    //0); receiver.volCoins = 1;

    req.session.account.volCoins -= amount;
    query = `UPDATE Users SET volCoins = "${req.session.account.volCoins}" WHERE username == "${req.session.account.username}";`;
    await db.exec(query);
    const receiverNewBal = receiver.volCoins + Math.abs(amount);
    query = `UPDATE Users SET volCoins = "${receiverNewBal}" WHERE username == "${receiver.username}";`;
    await db.exec(query);
    render(req, res, next, 'transfer/success', 'Transfer Complete', false, {receiver, amount});
  } else { // user does not exist
    let q = req.body.destination_username;
    if (q == null) q = '';

    let oldQ;
    while (q !== oldQ) {
      oldQ = q;
      q = q.replace(/script|SCRIPT|img|IMG/g, '');
    }
    render(req, res, next, 'transfer/form', 'Transfer VolCoins', `User ${q} does not exist!`, {receiver:null, amount:null});
  }
}));

module.exports = router;