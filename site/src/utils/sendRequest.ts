import {API} from "../API";

export async function sendRequest<T>(endpoint: string, payload?: string):Promise<T> {
    const res = await fetch(endpoint, { method: 'POST' });
    if (!res.ok)
        throw new Error('Failed to fetch: ' + endpoint);
    const data = await res.json();
    return data
}