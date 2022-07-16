import dns.query
import dns.name
import dns.message
import dns.rdata
import dns.rdatatype
import argparse

root_dns_server_ip = "198.41.0.4"

mapping_caches = {"localhost":"127.0.0.1"}

prevAuthority = dns.message.Message(None)

def dns_query(target_domain: str, dns_ip_addr: str) -> str:
    global prevAuthority
    ### check if the target domain's IP address is cached.
    if prevAuthority.section_from_number(1) != None:  # check if the target domain's IP address is cached.
        return prevAuthority.section_from_number(1)
    """
    DO SOMETHING FOR CHECKING THE TARGET DOMAIN IF BEING CACHED
    """

    ### if not, please perform all recursion for the target domain
    """
    1. Make a DNS query to be sent to a DNS server. In this homework, we may want to consider only A records. 
    2. Send the DNS query to server over UDP. The first should be sent to the root DNS server (198.41.0.4) 
    """
    query = dns.message.make_query(target_domain, dns.rdatatype.A)
    prevAuthority = dns.query.udp(query, dns_ip_addr)
    print(prevAuthority.section_from_number(1))

    """
    If you receive the responses from the DNS servers, the responses have two sections.
    1. Answer section: Records that answer the question
    2. Authority or/and Additional section: Records that point toward authoritative nameservers / Records that are related to the query
    For example, if you sent a query to a root DNS server for a certain domain, as the root dns server does not know the answer, the response
    will have no answer section; rather it has authrity section and additional section. In other words, it tells us about the authoritative 
    nameservers (NS Record) along with their IP addresses in the Additional section (A record).
    """

    ip_address_of_target_domain = dns_query(target_domain, "160.36.0.84")

    return ip_address_of_target_domain


if __name__ == "__main__":
    argument_parser = argparse.ArgumentParser()
    argument_parser.add_argument("name", nargs="+", help="target domain name to look up")
    program_args = argument_parser.parse_args()

    ipaddress_of_domain = dns_query(program_args.name[0], root_dns_server_ip)
    print(program_args.name[0], ipaddress_of_domain)

'''#print(prevAuthority.index[(2, "edu.", 1, 2, 0, None)])
    print(prevAuthority.section_from_number(1))
    #for key in prevAuthority.index:
        #print(key)
    #(<MessageSection.AUTHORITY: 2>, <DNS name edu.>, 1, 2, <RdataType.TYPE0: 0>, None)'''