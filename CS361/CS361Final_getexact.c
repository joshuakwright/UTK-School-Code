int getexact (counter *c) {
    pthread_mutex_lock(&c->glock);
    int val = c->global;
    pthread_mutex_unlock(&c->glock);
    
    for (int i = 0; i < NUMCPUS; i++) {
        pthread_mutex_lock(&c->llock[i]);
        val += c->local[i];
        pthread_mutex_unlock(&c->llock[i]);
    }
    return val;
}