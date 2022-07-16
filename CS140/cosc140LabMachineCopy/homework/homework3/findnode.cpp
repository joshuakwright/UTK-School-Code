list::node *list::findnode(int i) { 
	
	node *p;

	if (i >= N/2) {
		p = head->prev;
		i = n-1-i;
		while (i--) {
			p = p->prev;
		}
	}
	if (i < N/2) {
		while (i--) {
			p = p->next;
		}
	}
	return p;
}
